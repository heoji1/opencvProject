#include <iostream>
#include <opencv2/opencv.hpp>

#define IM_TYPE	CV_8UC3

using namespace cv;


// Note that this code is for the case when an input data is a color value.
int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat output;


	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", input);


	Mat samples(input.rows * input.cols, 5, CV_32F);
	float sigma = 1.0f;
	for (int y = 0; y < input.rows; y++) {
		for (int x = 0; x < input.cols; x++) {
			for (int z = 0; z < 3; z++) {
				samples.at<float>(y + x*input.rows, z) = input.at<Vec3b>(y, x)[z]/255.0f;
			}
			samples.at<float>(y + x*input.rows, 3) = (float)x / input.cols/sigma; //x 위치 값
			samples.at<float>(y + x*input.rows, 4) = (float)y / input.rows/sigma;
		}
	}

	// Clustering is performed for each channel (RGB)
	// Note that the intensity value is not normalized here (0~1). You should normalize both intensity and position when using them simultaneously.
	int clusterCount = 10;
	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, clusterCount, labels, TermCriteria(TermCriteria::COUNT | TermCriteria::EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(input.size(), input.type());
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x*input.rows, 0);
			//Fill code that finds for each pixel of each channel of the output image the intensity of the cluster center.
			new_image.at<Vec3b>(y,x)[0] = (centers.at<float>(cluster_idx, 0)*255.0f);
			new_image.at<Vec3b>(y,x)[1] = (centers.at<float>(cluster_idx, 1)*255.0f);
			new_image.at<Vec3b>(y,x)[2] = (centers.at<float>(cluster_idx, 2)*255.0f);
		}
	imshow("clustered image", new_image);

	waitKey(0);

	return 0;
}

