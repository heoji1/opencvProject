#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;


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

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", input);

    pyrMeanShiftFiltering(input, output, 31, 20, 5);
	imshow("Meanshift", output);

	Mat output_gray;
	cvtColor(output, output_gray, COLOR_RGB2GRAY);

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("Meanshift_Gray", WINDOW_AUTOSIZE);
	imshow("Meanshift_Gray", output_gray);


	waitKey();

	return 0;
}