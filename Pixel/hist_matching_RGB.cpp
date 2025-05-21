#include "hist_func.h"

void hist_match(Mat &input, Mat &matched, G *trans_func, float *CDF);

int main() {
    Mat input = imread("input.jpg", IMREAD_COLOR);
    Mat input_YUV;
    // 1) RGB -> YUV 변환
    cvtColor(input, input_YUV, COLOR_BGR2YUV);

    Mat intermediate_YUV = input_YUV.clone();
    Mat matched_YUV = input_YUV.clone();
    
    // Y, U, V 채널 분리
    Mat channels[3];
    split(input_YUV, channels);
    Mat Y = channels[0];  // Y 채널

    // PDF 및 전송 함수 파일 생성
    FILE *f_PDF_RGB_input;
    FILE *f_PDF_RGB_matched;
    FILE *f_trans_func_match_Y;
    
    fopen_s(&f_PDF_RGB_input, "original_PDF_RGB.txt", "w+");
    fopen_s(&f_PDF_RGB_matched, "output_PDF_RGB.txt", "w+");
    fopen_s(&f_trans_func_match_Y, "trans_func_match_Y.txt", "w+");
    
    // 원본 RGB 이미지의 PDF 계산
    float **PDF_RGB_input = cal_PDF_RGB(input);
    
    // Y 채널의 PDF와 CDF 계산
    float *PDF_Y = cal_PDF(Y);
    float *CDF_Y = cal_CDF(Y);
    
    // 1. 히스토그램 평활화 전송 함수 계산: s = T(r) = (L-1)*CDF
    G trans_func_inter[L] = { 0 };
    for (int i = 0; i < L; i++)
        trans_func_inter[i] = (G)((L - 1) * CDF_Y[i]);
    
    // 중간 과정 이미지 생성 (히스토그램 평활화)
    Mat intermediate_channels[3];
    split(intermediate_YUV, intermediate_channels);
    
    for (int i = 0; i < Y.rows; i++) {
        for (int j = 0; j < Y.cols; j++) {
            intermediate_channels[0].at<G>(i, j) = trans_func_inter[Y.at<G>(i, j)];
        }
    }
    
    merge(intermediate_channels, 3, intermediate_YUV);
    Mat intermediate;
    cvtColor(intermediate_YUV, intermediate, COLOR_YUV2BGR);
    
    // 2. Y 채널에만 히스토그램 매칭 적용
    G trans_func_match_Y[L] = { 0 };
    
    // 히스토그램 매칭 함수 호출
    hist_match(Y, channels[0], trans_func_match_Y, CDF_Y);
    
    // 채널 병합
    merge(channels, 3, matched_YUV);
    
    // 3) YUV -> RGB 변환
    Mat matched;
    cvtColor(matched_YUV, matched, COLOR_YUV2BGR);
    float **PDF_RGB_matched = cal_PDF_RGB(matched);
    
    // 파일에 데이터 저장
    for (int i = 0; i < L; i++) {
        // RGB 히스토그램 저장
        fprintf(f_PDF_RGB_input, "%d\t%f\t%f\t%f\n", i, 
                PDF_RGB_input[i][0], PDF_RGB_input[i][1], PDF_RGB_input[i][2]);
        fprintf(f_PDF_RGB_matched, "%d\t%f\t%f\t%f\n", i, 
                PDF_RGB_matched[i][0], PDF_RGB_matched[i][1], PDF_RGB_matched[i][2]);
        
        // Y 채널 전송 함수 저장
        fprintf(f_trans_func_match_Y, "%d\n", trans_func_match_Y[i]);
    }
    
    // 메모리 해제
    free(PDF_Y);
    for (int i = 0; i < L; i++) {
        free(PDF_RGB_input[i]);
        free(PDF_RGB_matched[i]);
    }
    free(PDF_RGB_input);
    free(PDF_RGB_matched);
    free(CDF_Y);
    
    fclose(f_PDF_RGB_input);
    fclose(f_PDF_RGB_matched);
    fclose(f_trans_func_match_Y);
    
    ////////////////////// Show each image ///////////////////////
    
    namedWindow("Input", WINDOW_AUTOSIZE);
    imshow("Input", input);
    
    namedWindow("Intermediate", WINDOW_AUTOSIZE);
    imshow("Intermediate", intermediate);
    
    namedWindow("Matched", WINDOW_AUTOSIZE);
    imshow("Matched", matched);
    
    //////////////////////////////////////////////////////////////
    
    waitKey(0);
    
    return 0;
}

void hist_match(Mat &input, Mat &matched, G *trans_func, float *CDF_input) {
    
    // calculate G^-1(s)
    G inverse_G[L] = { 0 };
    for (int i = 0; i < L; i++) {
        float approx = 1.0f; // 가장 근사 값으로 지정
        for (int z = 0; z < L; z++) {
            float a = abs(CDF_input[i] - (float)z / (L - 1));
            if (a < approx) {
                approx = a;
                inverse_G[i] = z;
            }
        }
    }
    
    // calculate z = G^-1(T(r))
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            G r = input.at<G>(i, j);
            matched.at<G>(i, j) = inverse_G[r]; // z 값을 matㄴched에 대입
            trans_func[r] = inverse_G[r]; 
        }
    }
}
