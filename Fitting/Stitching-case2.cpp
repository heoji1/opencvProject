#include <opencv2/opencv.hpp>
// #include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#define RATIO_THR 0.4

using namespace std;
using namespace cv;


template <typename T>
Mat cal_affine(int ptl_x[], int ptl_y[], int ptr_x[], int ptr_y[], int number_of_points);

template <typename T>
Mat ransac_affine(vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, int S, double thr, int k);
void blend_stitching(const Mat I1, const Mat I2, Mat &I_f, int diff_x, int diff_y, float alpha);

double euclidDistance(const Mat& vec1, const Mat& vec2);
int nearestNeighbor(const Mat& vec, const vector<KeyPoint>& keypoints, const Mat& descriptors);
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold);

int main() {

	Mat input1 = imread("input1.jpg", IMREAD_COLOR);
	Mat input2 = imread("input2.jpg", IMREAD_COLOR);
	Mat input1_gray, input2_gray;

	if (!input1.data || !input2.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	resize(input1, input1, Size(input1.cols / 2, input1.rows / 2));
	resize(input2, input2, Size(input2.cols / 2, input2.rows / 2));

	cvtColor(input1, input1_gray, COLOR_RGB2GRAY);
	cvtColor(input2, input2_gray, COLOR_RGB2GRAY);

	Ptr<SIFT> sift = SIFT::create(
		0,		// nFeatures
		4,		// nOctaveLayers
		0.04,	// contrastThreshold
		10,		// edgeThreshold
		1.6		// sigma
	);

	// Create a image for displaying mathing keypoints
	Size size = input2.size();
	Size sz = Size(size.width + input1_gray.size().width, max(size.height, input1_gray.size().height));
	Mat matchingImage = Mat::zeros(sz, CV_8UC3);

	input1.copyTo(matchingImage(Rect(size.width, 0, input1_gray.size().width, input1_gray.size().height)));
	input2.copyTo(matchingImage(Rect(0, 0, size.width, size.height)));
	
	// Compute keypoints and descriptor from the source image in advance
	vector<KeyPoint> keypoints1;
	Mat descriptors1;

	sift->detectAndCompute(input1_gray, noArray(), keypoints1, descriptors1);
	printf("input1 : %d keypoints are found.\n", (int)keypoints1.size());

	vector<KeyPoint> keypoints2;
	Mat descriptors2;

	// Detect keypoints

	sift->detectAndCompute(input2_gray, noArray(), keypoints2, descriptors2);
	printf("input2 : %zd keypoints are found.\n", keypoints2.size());

	for (int i = 0; i < keypoints1.size(); i++) {
		KeyPoint kp = keypoints1[i];
		kp.pt.x += size.width;
		circle(matchingImage, kp.pt, cvRound(kp.size*0.25), Scalar(255, 255, 0), 1, 8, 0);
	}

	for (int i = 0; i < keypoints2.size(); i++) {
		KeyPoint kp = keypoints2[i];
		circle(matchingImage, kp.pt, cvRound(kp.size*0.25), Scalar(255, 255, 0), 1, 8, 0);
	}

	// Find nearest neighbor pairs
	vector<Point2f> srcPoints;
	vector<Point2f> dstPoints;
	bool crossCheck = false;
	bool ratio_threshold = false;
	findPairs(keypoints1, descriptors1, keypoints2, descriptors2, srcPoints, dstPoints, crossCheck, ratio_threshold);


	// 이미지를 32비트 형식으로 변환
	input1.convertTo(input1, CV_32FC3, 1.0 / 255);
	input2.convertTo(input2, CV_32FC3, 1.0 / 255);

	// height(row), width(col) of each image
	const float I1_row = input1.rows;
	const float I1_col = input1.cols;
	const float I2_row = input2.rows;
	const float I2_col = input2.cols;

	//2. Affine transform (Case2)
    int n = (int)srcPoints.size();

	//RANSAC
	int S = 20;
	double thr = 3.0;
	int k = 3;

	Mat A12 = ransac_affine<float>(srcPoints, dstPoints, S, thr, k);
	Mat A21 = ransac_affine<float>(dstPoints, srcPoints, S, thr, k);

	// compute corners (p1, p2, p3, p4)
	// p1: (0,0)
	// p2: (row, 0)
	// p3: (row, col)
	// p4: (0, col)
	Point2f p1(A21.at<float>(0) * 0 + A21.at<float>(1) * 0 + A21.at<float>(2), A21.at<float>(3) * 0 + A21.at<float>(4) * 0 + A21.at<float>(5));
	Point2f p2(A21.at<float>(0) * 0 + A21.at<float>(1) * I2_row + A21.at<float>(2), A21.at<float>(3) * 0 + A21.at<float>(4) * I2_row + A21.at<float>(5));
	Point2f p3(A21.at<float>(0) * I2_col + A21.at<float>(1) * I2_row + A21.at<float>(2), A21.at<float>(3) * I2_col + A21.at<float>(4) * I2_row + A21.at<float>(5));
	Point2f p4(A21.at<float>(0) * I2_col + A21.at<float>(1) * 0 + A21.at<float>(2), A21.at<float>(3) * I2_col + A21.at<float>(4) * 0 + A21.at<float>(5));

	// compute boundary for merged image(I_f)
	// bound_u <= 0
	// bound_b >= I1_row-1
	// bound_l <= 0
	// bound_b >= I1_col-1
	int bound_u = (int)round(min(0.0f, min(p1.y, p4.y)));
	int bound_b = (int)round(max(I1_row-1, max(p2.y, p3.y)));
	int bound_l = (int)round(min(0.0f, min(p1.x, p2.x)));
	int bound_r = (int)round(max(I1_col-1, max(p3.x, p4.x)));
	
	// initialize merged image
	Mat I_f(bound_b - bound_u + 1, bound_r - bound_l + 1, CV_32FC3, Scalar(0));

	// inverse warping with bilinear interplolation
	for (int i = bound_u; i <= bound_b; i++) {
		for (int j = bound_l; j <= bound_r; j++) {
			float x = A12.at<float>(0) * j + A12.at<float>(1) * i + A12.at<float>(2) - bound_l;
			float y = A12.at<float>(3) * j + A12.at<float>(4) * i + A12.at<float>(5) - bound_u;

			float y1 = floor(y);
			float y2 = ceil(y);
			float x1 = floor(x);
			float x2 = ceil(x);

			float mu = y - y1;
			float lambda = x - x1;

			if (x1 >= 0 && x2 < I2_col && y1 >= 0 && y2 < I2_row)
				I_f.at<Vec3f>(i - bound_u, j - bound_l) = lambda * (mu * input2.at<Vec3f>(y2, x2) + (1 - mu) * input2.at<Vec3f>(y1, x2)) +
														  (1 - lambda) *(mu * input2.at<Vec3f>(y2, x1) + (1 - mu) * input2.at<Vec3f>(y1, x1));
		}
	}

	// image stitching with blend
	blend_stitching(input1, input2, I_f, bound_l, bound_u, 0.5);

	namedWindow("Case2 result");
	imshow("Case2 result", I_f);

	I_f.convertTo(I_f, CV_8UC3, 255.0);
	imwrite("result_case2.png", I_f);

	waitKey(0);
    
    return 0;
}

double euclidDistance(const Mat& vec1, const Mat& vec2) {
	double sum = 0.0;
	int dim = vec1.cols;
	for (int i = 0; i < dim; i++) {
		sum += (vec1.at<float>(0, i) - vec2.at<float>(0, i)) * (vec1.at<float>(0, i) - vec2.at<float>(0, i));
	}

	return sqrt(sum);
}


int nearestNeighbor(const Mat& vec, const vector<KeyPoint>& keypoints, const Mat& descriptors) {
	int neighbor = -1;
	double minDist = 1e6;

	for (int i = 0; i < descriptors.rows; i++) {
		Mat v = descriptors.row(i);		// each row of descriptor
		
		//
		//	Fill the code
		double Dist = euclidDistance(vec, v);
		if( Dist < minDist) {
			minDist = Dist;
			neighbor = i;
		}
		//

	}

	return neighbor;
}


void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold) {
	for (int i = 0; i < descriptors1.rows; i++) {
		KeyPoint pt1 = keypoints1[i];
		Mat desc1 = descriptors1.row(i);

		int nn = nearestNeighbor(desc1, keypoints2, descriptors2);

		// Refine matching points using ratio_based thresholding
		if (ratio_threshold) {

			vector<double> Dists;
			for (int j = 0; j < descriptors2.rows; j++) {
				Mat desc2 = descriptors2.row(j);
				double Dist = euclidDistance(desc1, desc2);
				Dists.push_back(Dist);
			}

			sort(Dists.begin(), Dists.end());
			double dist2 = Dists[1];

			double dist1 = euclidDistance(desc1, descriptors2.row(nn));
			if ((dist1 / dist2) > RATIO_THR) {
				continue;
			}
			//
		}


		// Refine matching points using cross-checking
		if (crossCheck) {
			//
			//	Fill the code
			Mat desc2 = descriptors2.row(nn);
			int nn2 = nearestNeighbor(desc2, keypoints1, descriptors1);

			if( nn2 != i) {
				continue;
			}
			//
		}
		KeyPoint pt2 = keypoints2[nn];
		srcPoints.push_back(pt1.pt);
		dstPoints.push_back(pt2.pt);
	}
}

template <typename T>
Mat cal_affine(int ptl_x[], int ptl_y[], int ptr_x[], int ptr_y[], int number_of_points) {

	Mat M(2 * number_of_points, 6, CV_32F, Scalar(0));
	Mat b(2 * number_of_points, 1, CV_32F);

	Mat M_trans, temp, affineM;

	// initialize matrix
	for (int i = 0; i < number_of_points; i++) {
		M.at<T>(2 * i, 0) = ptl_x[i];		M.at<T>(2 * i, 1) = ptl_y[i];		M.at<T>(2 * i, 2) = 1;
		M.at<T>(2 * i + 1, 3) = ptl_x[i];		M.at<T>(2 * i + 1, 4) = ptl_y[i];		M.at<T>(2 * i + 1, 5) = 1;
		b.at<T>(2 * i) = ptr_x[i];		b.at<T>(2 * i + 1) = ptr_y[i];
	}

	// (M^T * M)^(−1) * M^T * b ( * : Matrix multiplication)
	transpose(M, M_trans);
	invert(M_trans * M, temp);
	affineM = temp * M_trans * b;

	return affineM;
}


template <typename T>
Mat ransac_affine(vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, int S, double thr, int k) {

	int n = (int)srcPoints.size();

    int best_count = 0;
    Mat bestTrans;

    vector<bool> bestInlierFlags(n, false);

	RNG rng;

    for (int iter = 0; iter < S; iter++) {
        vector<int> idxs;

        int* ptl_x = new int [k];
        int* ptl_y = new int [k];
        int* ptr_x = new int [k];
        int* ptr_y = new int [k];

        while ((int)idxs.size() < k) {
			int idx = rng.uniform(0, n);

			bool only = true;

			for (int j = 0; j < idxs.size(); j++) {
				if (idxs[j] == idx) {
					only = false;
					break;
				}
			}

			if (only) {
				idxs.push_back(idx);
			}
		}

		for (int i = 0; i < k; i++) {
			ptl_x[i] = round(srcPoints[idxs[i]].x);
			ptl_y[i] = round(srcPoints[idxs[i]].y);
			ptr_x[i] = round(dstPoints[idxs[i]].x);
			ptr_y[i] = round(dstPoints[idxs[i]].y);
		}

        Mat Trans = cal_affine<float>(ptl_x, ptl_y, ptr_x, ptr_y, k); // 6 parameters

		delete[] ptl_x;
		delete[] ptl_y;
		delete[] ptr_x;
		delete[] ptr_y;

        vector<bool> current_inliers(n, false);

        int count = 0;

        for (int i = 0; i < n; i++) {
            float Px = Trans.at<float>(0)*srcPoints[i].x + Trans.at<float>(1)*srcPoints[i].y + Trans.at<float>(2);
            float Py = Trans.at<float>(3)*srcPoints[i].x + Trans.at<float>(4)*srcPoints[i].y + Trans.at<float>(5);

            float dx = Px - dstPoints[i].x;
            float dy = Py - dstPoints[i].y;

			float dist = sqrt(dx*dx + dy*dy);

            if (dist*dist < thr*thr) {
                current_inliers[i] = true;
                count++;
            }
        }

        if (count > best_count) {
            best_count = count;
            bestTrans = Trans.clone();
            bestInlierFlags = current_inliers;
        }
    }

    if (best_count > k) {
        int* ptl_x_in = new int [best_count];
        int* ptl_y_in = new int [best_count];
        int* ptr_x_in = new int [best_count];
        int* ptr_y_in = new int [best_count];

        int idx = 0;
        for (int i = 0; i < n; i++) {
            if (bestInlierFlags[i]) {
                ptl_x_in[idx] = round(srcPoints[i].x);
                ptl_y_in[idx] = round(srcPoints[i].y);
                ptr_x_in[idx] = round(dstPoints[i].x);
                ptr_y_in[idx] = round(dstPoints[i].y);
                idx++;
            }
        }

        Mat refineTrans = cal_affine<float>(ptl_x_in, ptl_y_in, ptr_x_in, ptr_y_in, best_count);

		delete[] ptl_x_in;
		delete[] ptl_y_in;
		delete[] ptr_x_in;
		delete[] ptr_y_in;

        bestTrans = refineTrans;
    }
	
    return bestTrans;
}

void blend_stitching(const Mat I1, const Mat I2, Mat &I_f, int bound_l, int bound_u, float alpha) {

	int col = I_f.cols;
	int row = I_f.rows;

	// I2 is already in I_f by inverse warping
	for (int i = 0; i < I1.rows; i++) {
		for (int j = 0; j < I1.cols; j++) {
			bool cond_I2 = I_f.at<Vec3f>(i - bound_u, j - bound_l) != Vec3f(0, 0, 0) ? true : false;

			if (cond_I2)
				I_f.at<Vec3f>(i - bound_u, j - bound_l) = alpha * I1.at<Vec3f>(i, j) + (1 - alpha) * I_f.at<Vec3f>(i - bound_u, j - bound_l);
			else
				I_f.at<Vec3f>(i - bound_u, j - bound_l) = I1.at<Vec3f>(i, j);

		}
	}
}
