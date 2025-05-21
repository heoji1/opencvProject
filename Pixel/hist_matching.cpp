#include "hist_func.h"

void hist_match(Mat &input, Mat &matched, G *trans_func, float *CDF);

int main() {

    Mat input = imread("input.jpg", IMREAD_COLOR);

	cvtColor(input, input, COLOR_RGB2GRAY); // RGB -> Grayscale

    Mat intermediate = input.clone();
    Mat matched = input.clone();

    FILE *f_PDF_input;
    FILE *f_PDF_matched;
	FILE *f_trans_func_match;

    fopen_s(&f_PDF_input, "original_PDF.txt", "w");
    fopen_s(&f_PDF_matched, "output_PDF.txt", "w");
	fopen_s(&f_trans_func_match, "trans_func_match.txt", "w+");
    
    // input 이미지의 PDF,CDF
    float *PDF_input = cal_PDF(input);
    float *CDF_input = cal_CDF(input);
    
    // 1. s = T(r) = (L-1)*CDF
    G trans_func_inter[L] = { 0 };
    for (int i = 0; i < L; i++)
        trans_func_inter[i] = (G)((L - 1) * CDF_input[i]);
    
    // 2. histogram equalization 
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            intermediate.at<G>(i, j) = trans_func_inter[input.at<G>(i, j)];
        }
    }
    
    // 3. s = G(z) -> z = G^-1(s) =G^-1(T(r))
    G trans_func_match[L] = { 0 };

    hist_match(input, matched, trans_func_match, CDF_input);
    float *matched_PDF_gray = cal_PDF(matched);
    
    for (int i = 0; i < L; i++) {
        fprintf(f_PDF_input, "%d\t%f\n", i, PDF_input[i]);
        fprintf(f_PDF_matched, "%d\t%f\n", i, matched_PDF_gray[i]);

        fprintf(f_trans_func_match, "%d\n", trans_func_match[i]);  
    }
    
    // memory release
    free(PDF_input);
    free(CDF_input);
    fclose(f_PDF_input);
    fclose(f_PDF_matched);
    fclose(f_trans_func_match);
    
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
    
    // z = G^-1(s)
    G inverse_G[L] = { 0 };
    for (int i = 0; i < L; i++) {
        float approx = 1.0f; // 가장 근사 값으로 지정
        for (int z = 0; z < L; z++) {
            // s = G(z)의 값을 계산한 후, 출력된 s(여기서는 a)의 근사값을 결과로 갖는 z 값을 찾아냄
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
            matched.at<G>(i, j) = inverse_G[r]; //z 값을 matched에 대입
            trans_func[r] = inverse_G[r]; 
        }
    }
}
