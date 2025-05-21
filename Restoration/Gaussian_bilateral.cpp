#include <opencv2/opencv.hpp>
#include <stdio.h>

#define IM_TYPE	CV_64FC3

using namespace cv;

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma);
Mat Bilateralfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char *opt);
Mat Bilateralfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char *opt);

int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat input_gray;

	// check for validation
	if (!input.data) {
		printf("Could not open\n");
		return -1;
	}

	cvtColor(input, input_gray, COLOR_RGB2GRAY);	// convert RGB to Grayscale
	
	// 8-bit unsigned char -> 64-bit floating point
	input.convertTo(input, CV_64FC3, 1.0 / 255);
	input_gray.convertTo(input_gray, CV_64F, 1.0 / 255);

	// Add noise to original image
	Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);
	Mat noise_RGB = Add_Gaussian_noise(input, 0, 0.1);

	// Denoise, using bilateral filter
	Mat Denoised_Gray = Bilateralfilter_Gray(noise_Gray, 3, 10, 10, 10, "zero-padding");
	Mat Denoised_RGB = Bilateralfilter_RGB(noise_RGB, 3, 10, 10, 10, "adjustkernel");

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	namedWindow("Gaussian Noise (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Gaussian Noise (Grayscale)", noise_Gray);

	namedWindow("Gaussian Noise (RGB)", WINDOW_AUTOSIZE);
	imshow("Gaussian Noise (RGB)", noise_RGB);

	namedWindow("Denoised (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Denoised (Grayscale)", Denoised_Gray);

	namedWindow("Denoised (RGB)", WINDOW_AUTOSIZE);
	imshow("Denoised (RGB)", Denoised_RGB);

	waitKey(0);

	return 0;
}

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma) {

	Mat NoiseArr = Mat::zeros(input.rows, input.cols, input.type());
	RNG rng;
	rng.fill(NoiseArr, RNG::NORMAL, mean, sigma);

	add(input, NoiseArr, NoiseArr);

	return NoiseArr;
}

Mat Bilateralfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char *opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;
	double kernelvalue;

	double spatial_dist;
	double intensity_dist;

	// Initialiazing Gaussian Kernel Matrix
	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			double sum = 0.0;
			double w = 0.0;

			double refer = (double)(input.at<double>(i, j));

			if (!strcmp(opt, "zero-padding")) {

				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						spatial_dist = exp(-(pow(x, 2) / (2 * pow(sigma_s, 2))) - (pow(y, 2) / (2 * pow(sigma_t, 2))));
						
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {
							double neighbor = (double)(input.at<double>(i + x, j + y));
							
							intensity_dist = exp(-(pow(refer- neighbor, 2) / (2 * pow(sigma_r, 2))));
							
							kernelvalue = spatial_dist * intensity_dist;
							
							sum += kernelvalue * neighbor;
							w += kernelvalue;
						}

					}
				}

			}

			else if (!strcmp(opt, "mirroring")) {

				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						spatial_dist = exp(-(pow(x, 2) / (2 * pow(sigma_s, 2))) - (pow(y, 2) / (2 * pow(sigma_t, 2))));

						if (i + x > row - 1) {  //mirroring for the border pixels
							tempa = i - x;
						}
						else if (i + x < 0) {
							tempa = -(i + x);
						}
						else {
							tempa = i + x;
						}
						if (j + y > col - 1) {
							tempb = j - y;
						}
						else if (j + y < 0) {
							tempb = -(j + y);
						}
						else {
							tempb = j + y;
						}
						double neighbor = (double)(input.at<double>(tempa, tempb));

						intensity_dist = exp(-(pow(refer- neighbor, 2) / (2 * pow(sigma_r, 2))));
							
						kernelvalue = spatial_dist * intensity_dist;
							
						sum += kernelvalue * neighbor;
						w += kernelvalue;
					}
				}

			}

			else if (!strcmp(opt, "adjustkernel")) {
				double sum2 = 0.0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						spatial_dist = exp(-(pow(x, 2) / (2 * pow(sigma_s, 2))) - (pow(y, 2) / (2 * pow(sigma_t, 2))));
						
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {
							double neighbor = (double)(input.at<double>(i + x, j + y));
							
							intensity_dist = exp(-(pow(refer- neighbor, 2) / (2 * pow(sigma_r, 2))));
							
							kernelvalue = spatial_dist * intensity_dist;
							
							sum += kernelvalue * neighbor;
							w += kernelvalue;
						}
					}
				}
			}
			output.at<double>(i, j) = (double)(sum / w);

		}
	}

	return output;
}

Mat Bilateralfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char *opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;
	double kernelvalue;

	double spatial_dist;
	double intensity_dist;

	// Initialiazing Gaussian Kernel Matrix
	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			double sum_r = 0.0;
			double sum_g = 0.0;
			double sum_b = 0.0;

			double w = 0.0;

			double refer_r = (double)(input.at<Vec3d>(i,j)[0]);
			double refer_g = (double)(input.at<Vec3d>(i,j)[1]);
			double refer_b = (double)(input.at<Vec3d>(i,j)[2]);

			if (!strcmp(opt, "zero-padding")) {

				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						spatial_dist = exp(-(pow(x, 2) / (2 * pow(sigma_s, 2))) - (pow(y, 2) / (2 * pow(sigma_t, 2))));
						
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {

							double neighbor_r = (double)(input.at<Vec3d>(i + x, j + y)[0]);
							double neighbor_g = (double)(input.at<Vec3d>(i + x, j + y)[1]);
							double neighbor_b = (double)(input.at<Vec3d>(i + x, j + y)[2]);

							double C_dist = sqrt(pow(refer_r - neighbor_r,2)+pow(refer_g - neighbor_g,2)+pow(refer_b - neighbor_b,2));
							
							intensity_dist = exp(-(pow(C_dist, 2) / (2 * pow(sigma_r, 2))));
							
							kernelvalue = spatial_dist * intensity_dist;
							
							sum_r += kernelvalue * neighbor_r;
							sum_g += kernelvalue * neighbor_g;
							sum_b += kernelvalue * neighbor_b;

							w += kernelvalue;
						}

					}
				}

			}

			else if (!strcmp(opt, "mirroring")) {

				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						spatial_dist = exp(-(pow(x, 2) / (2 * pow(sigma_s, 2))) - (pow(y, 2) / (2 * pow(sigma_t, 2))));

						if (i + x > row - 1) {  //mirroring for the border pixels
							tempa = i - x;
						}
						else if (i + x < 0) {
							tempa = -(i + x);
						}
						else {
							tempa = i + x;
						}
						if (j + y > col - 1) {
							tempb = j - y;
						}
						else if (j + y < 0) {
							tempb = -(j + y);
						}
						else {
							tempb = j + y;
						}
						double neighbor_r = (double)(input.at<Vec3d>(tempa, tempb)[0]);
						double neighbor_g = (double)(input.at<Vec3d>(tempa, tempb)[1]);
						double neighbor_b = (double)(input.at<Vec3d>(tempa, tempb)[2]);

						double C_dist = sqrt(pow(refer_r - neighbor_r,2)+pow(refer_g - neighbor_g,2)+pow(refer_b - neighbor_b,2));
							
						intensity_dist = exp(-(pow(C_dist, 2) / (2 * pow(sigma_r, 2))));
							
						kernelvalue = spatial_dist * intensity_dist;
							
						sum_r += kernelvalue * neighbor_r;
						sum_g += kernelvalue * neighbor_g;
						sum_b += kernelvalue * neighbor_b;

						w += kernelvalue;
					}
				}

			}

			else if (!strcmp(opt, "adjustkernel")) {
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						spatial_dist = exp(-(pow(x, 2) / (2 * pow(sigma_s, 2))) - (pow(y, 2) / (2 * pow(sigma_t, 2))));
						
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {
							double neighbor_r = (double)(input.at<Vec3d>(i + x, j + y)[0]);
							double neighbor_g = (double)(input.at<Vec3d>(i + x, j + y)[1]);
							double neighbor_b = (double)(input.at<Vec3d>(i + x, j + y)[2]);

							double C_dist = sqrt(pow(refer_r - neighbor_r,2)+pow(refer_g - neighbor_g,2)+pow(refer_b - neighbor_b,2));
							
							intensity_dist = exp(-(pow(C_dist, 2) / (2 * pow(sigma_r, 2))));
							
							kernelvalue = spatial_dist * intensity_dist;
							
							sum_r += kernelvalue * neighbor_r;
							sum_g += kernelvalue * neighbor_g;
							sum_b += kernelvalue * neighbor_b;

							w += kernelvalue;
						}
					}
				}
			}
			output.at<Vec3d>(i,j)[0] = (double)(sum_r / w);
			output.at<Vec3d>(i,j)[1] = (double)(sum_g / w);
			output.at<Vec3d>(i,j)[2] = (double)(sum_b / w);
		}
	}

	return output;
}