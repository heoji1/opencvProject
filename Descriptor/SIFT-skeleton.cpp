#include <opencv2/opencv.hpp>
// #include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#define RATIO_THR 0.4

using namespace std;
using namespace cv;

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
	findPairs(keypoints2, descriptors2, keypoints1, descriptors1, srcPoints, dstPoints, crossCheck, ratio_threshold);
	printf("%zd keypoints are matched.\n", srcPoints.size());

	// Draw line between nearest neighbor pairs
	for (int i = 0; i < (int)srcPoints.size(); ++i) {
		Point2f pt1 = srcPoints[i];
		Point2f pt2 = dstPoints[i];
		Point2f from = pt1;
		Point2f to = Point(size.width + pt2.x, pt2.y);
		line(matchingImage, from, to, Scalar(0, 0, 255));
	}

	// Display mathing image
	namedWindow("Matching");
	imshow("Matching", matchingImage);


	waitKey(0);
	return 0;
}

/**
* Calculate euclid distance
*/
double euclidDistance(const Mat& vec1, const Mat& vec2) {
	double sum = 0.0;
	int dim = vec1.cols;
	for (int i = 0; i < dim; i++) {
		sum += (vec1.at<float>(0, i) - vec2.at<float>(0, i)) * (vec1.at<float>(0, i) - vec2.at<float>(0, i));
	}

	return sqrt(sum);
}

/**
* Find the index of nearest neighbor point from keypoints.
*/
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

/**
* Find pairs of points with the smallest distace between them
*/
// keypoints1, descriptors1: src
// keypoints2, descripyots2: dst
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold) {
	for (int i = 0; i < descriptors1.rows; i++) {
		KeyPoint pt1 = keypoints1[i];
		Mat desc1 = descriptors1.row(i);

		int nn = nearestNeighbor(desc1, keypoints2, descriptors2);

		// Refine matching points using ratio_based thresholding
		if (ratio_threshold) {
			//
			//	Fill the code
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