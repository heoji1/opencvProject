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
	
	Mat input_gray;
	cvtColor(input, input_gray, COLOR_RGB2GRAY);

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);


	Mat samples(input_gray.rows * input_gray.cols, 3, CV_32F);
	float sigma = 1.0f;
	for (int y = 0; y < input_gray.rows; y++) {
		for (int x = 0; x < input_gray.cols; x++) {

			samples.at<float>(y + x*input_gray.rows, 0) = ((float)(input_gray.at<uchar>(y, x))/255.0f);

			//위치 정보 추가 (정규화, 균형을 위한 상수 시그마)
			samples.at<float>(y + x*input_gray.rows, 1) = (float)x / input_gray.cols/sigma; //x 위치 값
			samples.at<float>(y + x*input_gray.rows, 2) = (float)y / input_gray.rows/sigma; //y 위치 값
		}
	}

	// Clustering is performed for each channel (RGB)
	// Note that the intensity value is not normalized here (0~1). You should normalize both intensity and position when using them simultaneously.
	int clusterCount = 20;
	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, clusterCount, labels, TermCriteria(TermCriteria::COUNT | TermCriteria::EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(input_gray.size(), input_gray.type());
	for (int y = 0; y < input_gray.rows; y++)
		for (int x = 0; x < input_gray.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x*input_gray.rows, 0);
			//Fill code that finds for each pixel of each channel of the output image the intensity of the cluster center.
			new_image.at<uchar>(y,x) = (centers.at<float>(cluster_idx, 0)*255.0f);
		}
	imshow("clustered image", new_image);

	waitKey(0);

	return 0;
}

