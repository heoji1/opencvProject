#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>       /* exp */
#define IM_TYPE	CV_8UC3

using namespace cv;

// Image Type
// "G" for GrayScale Image, "C" for Color Image
#if (IM_TYPE == CV_8UC3)
typedef uchar G;
typedef cv::Vec3b C;
#elif (IM_TYPE == CV_16SC3)
typedef short G;
typedef Vec3s C;
#elif (IM_TYPE == CV_32SC3)
typedef int G;
typedef Vec3i C;
#elif (IM_TYPE == CV_32FC3)
typedef float G;
typedef Vec3f C;
#elif (IM_TYPE == CV_64FC3)
typedef double G;
typedef Vec3d C;
#endif

Mat sobelfilter(const Mat input);

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
	output = sobelfilter(input); //Boundary process: zero-paddle, mirroring, adjustkernel

	namedWindow("Sobel Filter", WINDOW_AUTOSIZE);
	imshow("Sobel Filter", output);


	waitKey(0);

	return 0;
}


Mat sobelfilter(const Mat input) {

	Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int n = 1; // Sobel Filter Kernel N

	// Initialiazing 2 Kernel Matrix with 3x3 size for Sx and Sy
	//Fill code to initialize Sobel filter kernel matrix for Sx and Sy (Given in the lecture notes)
	int Sx[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};

	int Sy[3][3] = {
		{-1, -2, -1},
		{0, 0, 0},
		{1, 2, 1}
	};

	Mat output = Mat::zeros(row, col, input.type());


	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			float Ix_r = 0.0;
            float Ix_g = 0.0;
            float Ix_b = 0.0;

			float Iy_r = 0.0;
            float Iy_g = 0.0;
            float Iy_b = 0.0;

			for (int a = -n; a <= n; a++) {
				for (int b = -n; b <= n; b++) {
					// Use mirroring boundary process
					int tempa;
					int tempb;

					if (i + a > row - 1) {  //mirroring for the border pixels
						tempa = i - a;
					}
					else if (i + a < 0) {
						tempa = -(i + a);
					}
					else {
						tempa = i + a;
					}

					if (j + b > col - 1) {
						tempb = j - b;
					}
					else if (j + b < 0) {
						tempb = -(j + b);
					}
					else {
						tempb = j + b;
					}

					Ix_r += Sx[a+n][b+n]*(float)(input.at<C>(tempa, tempb)[0]);
                    Ix_g += Sx[a+n][b+n]*(float)(input.at<C>(tempa, tempb)[1]);
                    Ix_b += Sx[a+n][b+n]*(float)(input.at<C>(tempa, tempb)[2]);

					Iy_r += Sy[a+n][b+n]*(float)(input.at<C>(tempa, tempb)[0]);
                    Iy_g += Sy[a+n][b+n]*(float)(input.at<C>(tempa, tempb)[1]);
                    Iy_b += Sy[a+n][b+n]*(float)(input.at<C>(tempa, tempb)[2]);

				}
			}
			
			// Find output M(x,y) = sqrt( input.at<G>(x, y)*Sx + input.at<G>(x, y)*Sy )
			float M_r = sqrt(Ix_r*Ix_r+Iy_r*Iy_r);
            float M_g = sqrt(Ix_g*Ix_g+Iy_g*Iy_g);
            float M_b = sqrt(Ix_b*Ix_b+Iy_b*Iy_b);

            if (M_r > 255) {
                M_r = 255;
            }

            if (M_g > 255) {
                M_g = 255;
            }

            if (M_b > 255) {
                M_b = 255;
            }

			float M = (M_r + M_g + M_b) / 3;

            output.at<C>(i,j)[0] = (G)(M);
            output.at<C>(i,j)[1] = (G)(M);
            output.at<C>(i,j)[2] = (G)(M);
        }
    }
    return output;
}