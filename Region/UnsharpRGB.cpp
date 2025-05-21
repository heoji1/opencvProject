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

Mat gaussianfilter_sep(const Mat& input, int n, float sigmaT, float sigmaS, const char* opt);

Mat unsharpMask(const Mat& input, int n, float sigmaT, float sigmaS, const char* opt, float k);

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
	output = unsharpMask(input, 1,1,1,"mirroring",0.5);

	namedWindow("Unsharp Masking", WINDOW_AUTOSIZE);
	imshow("Unsharp Masking", output);


	waitKey(0);

	return 0;
}


Mat gaussianfilter_sep(const Mat& input, int n, float sigmaT, float sigmaS, const char* opt) {

	Mat kernel_s;
    Mat kernel_t;
	
	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;

	float denom_s;
    float denom_t;

	float kernelvalue;
    
    // 수직 커널 (y축?)
    kernel_s = Mat::zeros(kernel_size, 1, CV_32F);

    denom_s = 0.0;
    for (int a = -n; a <= n; a++) {
        float value = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))));
        kernel_s.at<float>(a+n, 0) = value;
        denom_s += value;
    }
    
    // 정규화
    for (int a = -n; a <= n; a++) {
        kernel_s.at<float>(a+n, 0) /= denom_s;
    }
    
    // 수평 커널(x축?)
    kernel_t = Mat::zeros(1, kernel_size, CV_32F);

    denom_t = 0.0;
    for (int b = -n; b <= n; b++) {
        float value = exp(-(pow(b, 2) / (2 * pow(sigmaT, 2))));
        kernel_t.at<float>(0, b+n) = value;
        denom_t += value;
    }
    
    for (int b = -n; b <= n; b++) {
        kernel_t.at<float>(0, b+n) /= denom_t;
    }
    

    // 중간 결과 저장용 행렬
    Mat inter_output = Mat::zeros(row, col, input.type());
    
    // 1단계: 수직 방향 필터링
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            float sum_r = 0.0;
            float sum_g = 0.0;
            float sum_b = 0.0;
            
            if (!strcmp(opt, "zero-paddle")) {
                for (int a = -n; a <= n; a++) {

                    kernelvalue = kernel_s.at<float>(a+n, 0);

                    if ((i + a <= row - 1) && (i + a >= 0)) {
                        sum_r += kernelvalue * (float)(input.at<C>(i + a, j)[0]);
                        sum_g += kernelvalue * (float)(input.at<C>(i + a, j)[1]);
                        sum_b += kernelvalue * (float)(input.at<C>(i + a, j)[2]);
                    }
                }
            }

            else if (!strcmp(opt, "mirroring")) {
                for (int a = -n; a <= n; a++) {

                    kernelvalue = kernel_s.at<float>(a+n, 0);

                    if (i + a > row - 1) {
                        tempa = i - a;
                    }
                    else if (i + a < 0) {
                        tempa = -(i + a);
                    }
                    else {
                        tempa = i + a;
                    }

                    sum_r += kernelvalue*(float)(input.at<C>(tempa, j)[0]);
                    sum_g += kernelvalue*(float)(input.at<C>(tempa, j)[1]);
                    sum_b += kernelvalue*(float)(input.at<C>(tempa, j)[2]);
                }
            }
            else if (!strcmp(opt, "adjustkernel")) {
                float sum2 = 0.0;
                for (int a = -n; a <= n; a++) {

                    kernelvalue = kernel_s.at<float>(a+n, 0);

                    if ((i + a <= row - 1) && (i + a >= 0)) {
                        sum_r += kernelvalue*(float)(input.at<C>(i + a, j)[0]);
                        sum_g += kernelvalue*(float)(input.at<C>(i + a, j)[1]);
                        sum_b += kernelvalue*(float)(input.at<C>(i + a, j)[2]);
                        sum2 += kernel_s.at<float>(a+n, 0);
                    }
                }
                sum_r = sum_r / sum2;
                sum_g = sum_g / sum2;
                sum_b = sum_b / sum2;
            }
            
            inter_output.at<C>(i, j)[0] = (G)(sum_r);
			inter_output.at<C>(i, j)[1] = (G)(sum_g);
			inter_output.at<C>(i, j)[2] = (G)(sum_b);
        }
    }
    
    // 2단계: 수평 방향 필터링 (이전 결과 사용)
    Mat output = Mat::zeros(row, col, input.type());


    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            float sum_r = 0.0;
            float sum_g = 0.0;
            float sum_b = 0.0;
            
            if (!strcmp(opt, "zero-paddle")) {
                for (int b = -n; b <= n; b++) {
                    if ((j + b <= col - 1) && (j + b >= 0)) {

                        kernelvalue = kernel_t.at<float>(0, b+n);

                        sum_r += kernelvalue * (float)(inter_output.at<C>(i, j + b)[0]);
                        sum_g += kernelvalue * (float)(inter_output.at<C>(i, j + b)[1]);
                        sum_b += kernelvalue * (float)(inter_output.at<C>(i, j + b)[2]);
                    }
                }
            }
            else if (!strcmp(opt, "mirroring")) {
                for (int b = -n; b <= n; b++) {

                    kernelvalue = kernel_t.at<float>(0, b+n);

                    if (j + b > col - 1) {
                        tempb = j - b;
                    }
                    else if (j + b < 0) {
                        tempb = -(j + b);
                    }
                    else {
                        tempb = j + b;
                    }
                    sum_r += kernelvalue * (float)(inter_output.at<C>(i, tempb)[0]);
                    sum_g += kernelvalue * (float)(inter_output.at<C>(i, tempb)[1]);
                    sum_b += kernelvalue * (float)(inter_output.at<C>(i, tempb)[2]);
                }
            }
            else if (!strcmp(opt, "adjustkernel")) {
                float sum2 = 0.0;
                for (int b = -n; b <= n; b++) {

                    kernelvalue = kernel_t.at<float>(0, b+n);

                    if ((j + b <= col - 1) && (j + b >= 0)) {
                        sum_r += kernelvalue * (float)(inter_output.at<C>(i, j + b)[0]);
                        sum_g += kernelvalue * (float)(inter_output.at<C>(i, j + b)[0]);
                        sum_b += kernelvalue * (float)(inter_output.at<C>(i, j + b)[0]);
                        sum2 += kernelvalue;
                    }
                }
                sum_r = sum_r / sum2;
                sum_g = sum_g / sum2;
                sum_b = sum_b / sum2;
            }
            
            output.at<C>(i, j)[0] = (G)(sum_r);
            output.at<C>(i, j)[1] = (G)(sum_g);
            output.at<C>(i, j)[2] = (G)(sum_b);
        }
    }
    
    return output;
}


// 언샤프 마스킹 함수
Mat unsharpMask(const Mat& input, int n, float sigmaT, float sigmaS, const char* opt, float k) {

	int row = input.rows;
    int col = input.cols;

    // 가우시안 블러 적용
    Mat gaussian = gaussianfilter_sep(input, n, sigmaT, sigmaS, opt);

    // 언샤프 마스킹 계산
    Mat output = Mat::zeros(input.size(), input.type());

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            float I_r = (float)(input.at<C>(i, j)[0]);
            float I_g = (float)(input.at<C>(i, j)[1]);
            float I_b = (float)(input.at<C>(i, j)[2]);

            float L_r = (float)(gaussian.at<C>(i, j)[0]);
            float L_g = (float)(gaussian.at<C>(i, j)[1]);
            float L_b = (float)(gaussian.at<C>(i, j)[2]);

            float result_r = (I_r -k*L_r) / (1 - k);
            float result_g = (I_g -k*L_g) / (1 - k);
            float result_b = (I_b -k*L_b) / (1 - k);

            if (result_r < 0) {
                result_r = 0;
            }
            else if (result_r > 255) {
                result_r = 255;
            }

            if (result_g < 0) {
                result_g = 0;
            }
            else if (result_g > 255) {
                result_g = 255;
            }

            if (result_b < 0) {
                result_b = 0;
            }
            else if (result_b > 255) {
                result_b = 255;
            }

            output.at<C>(i,j)[0] = (G)(result_r);
            output.at<C>(i,j)[1] = (G)(result_g);
            output.at<C>(i,j)[2] = (G)(result_b);
        }
    }

    return output;
}