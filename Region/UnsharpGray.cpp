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
	Mat input_gray;
	Mat output;


	cvtColor(input, input_gray, COLOR_RGB2GRAY);


	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);
	output = unsharpMask(input_gray, 1,1,1,"mirroring",0.5);

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
            float sum = 0.0;
            
            if (!strcmp(opt, "zero-paddle")) {
                for (int a = -n; a <= n; a++) {

                    kernelvalue = kernel_s.at<float>(a+n, 0);

                    if ((i + a <= row - 1) && (i + a >= 0)) {
                        sum += kernelvalue * (float)(input.at<G>(i + a, j));
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
                    sum += kernelvalue * (float)(input.at<G>(tempa, j));
                }
            }
            else if (!strcmp(opt, "adjustkernel")) {
                float sum2 = 0.0;
                for (int a = -n; a <= n; a++) {

                    kernelvalue = kernel_s.at<float>(a+n, 0);

                    if ((i + a <= row - 1) && (i + a >= 0)) {
                        sum += kernelvalue * (float)(input.at<G>(i + a, j));
                        sum2 += kernelvalue;
                    }
                }
                sum = sum / sum2;
            }
            
            inter_output.at<G>(i, j) = (G)(sum);
        }
    }
    
    // 2단계: 수평 방향 필터링 (이전 결과 사용)
	Mat output = Mat::zeros(row, col, input.type());


    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            float sum = 0.0;
            
            if (!strcmp(opt, "zero-paddle")) {
                for (int b = -n; b <= n; b++) {

                    kernelvalue = kernel_t.at<float>(0, b+n);

                    if ((j + b <= col - 1) && (j + b >= 0)) {
                        sum += kernelvalue * (float)(inter_output.at<G>(i, j + b));
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
                    sum += kernelvalue * (float)(inter_output.at<G>(i, tempb));
                }
            }
            else if (!strcmp(opt, "adjustkernel")) {
                float sum2 = 0.0;
                for (int b = -n; b <= n; b++) {

                    kernelvalue = kernel_t.at<float>(0, b+n);

                    if ((j + b <= col - 1) && (j + b >= 0)) {
                        sum += kernelvalue * (float)(inter_output.at<G>(i, j + b));
                        sum2 += kernelvalue;
                    }
                }
                sum = sum / sum2;
            }
            
            output.at<G>(i, j) = (G)(sum);
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
            float I = (float)(input.at<G>(i, j));
            float L = (float)(gaussian.at<G>(i, j));

            float result = (I-k*L)/(1-k);

            // 값 범위 조정
			if(result < 0) {
				result = 0;
			}
			else if(result > 255) {
				result = 255;
			}

            output.at<G>(i, j) = (G)(result);
        }
    }

    return output;
}