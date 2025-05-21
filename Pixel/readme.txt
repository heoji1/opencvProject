<Pixel-wise Operation>
  
1. 코드의 목적과 흐름
    A. PDF & CDF (PDF_CDF.cpp)
        A-1. 목적: 이미지의 PDF와 CDF를 계산
        A-2. 코드 흐름
            1) grayscale 이미지로 변환
            2) cal_PDF, cal_CDF 함수를 사용하여 PDF와 CDF 계산
            3) 계산한 PDF, CDF를 텍스트 파일로 출력

    B. histogram stretching (hist_stretching.cpp)
        B-1. 목적: histogram stretching을 통해 이미지의 대비를 향상
        B-2. 코드 흐름
            1) grayscale 이미지로 변환
            2) linear stretching function을 적용하여 입력 범위(x1~x2)를 출력 범위(y1~y2)로 매핑 

    C. histogram equalization
        a. Grayscale에 대한 hist_eq (hist_eq.cpp)
            a-1. 목적: grayscale 이미지에 대한 histogram equalization을 수행
            a-2. 코드 흐름
                1) grayscale 이미지의 PDF, CDF 계산
                2) s = T(r) = (L-1)*CDF(r) 계산

        b. RGB color image에 대한 hist_eq (hist_eq_RGB.cpp)
            b-1. 목적: RGB 컬러 이미지에 대한 histogram equalization을 수행
            b-2. 코드 흐름
                1) RGB channel 별로 PDF, CDF 계산
                2) 각 channel에 독립적으로 histogram equalization 적용

        c. hist_eq_YUV.cpp
            c-1. 목적: YUV 컬러 이미지에서 Y(intenstiy)에 대한 histogram equalization을 수행
            c-2. 코드 흐름
                1) Y channel의 PDF, CDF 계산
                2) Y channel에만 histogram equalization 적용

    D. histogram matching 
        a. Grayscale에 대한 histogram matching (hist_matching.cpp)
            a-1. 목적: grayscale 이미지에 대한 histogram matching 수행
            a-2. 코드 흐름
                1) grayscale 이미지의 PDF, CDF 계산
                2) s = T(r) 계산
                3) z = G^-1(s) 계산
                4) z = G^-1(T(r)) 적용

        b. hist_matching_RGB
            b-1. 목적: YUV 컬러 이미지에서 Y(intensity)에 대한 histogram matching 수행
            b-2. 코드 흐름
                1) RGB 이미지를 YUV로 변환
                2) Y channel에 histogram matching 적용
                3) YUV 이미지를 다시 RGB로 변환

2. 실행 방법: input 이미지를 입력한 후 실행한다.

3. 매개 변수의 조정 방법
    A. hist_func.h
        cal_PDF: 각 픽셀 값의 빈도를 게산한 후 전체 픽셀 수로 나누어 확률 계산
        cal_CDF: PDF 값을 계산한 후, 반복문을 통해 현재 값 이하의 모든 확률을 합산

    B. hist_stretching.cpp
        linear_stretching 함수
            x1~x2 : 입력 이미지의 강도 범위
            y1~y2 : 출력 이미지의 강도 범위
            -> y = (b_{i+1} - b_i) / (a_{i+1} - a_i) * (x - a_i) + b_i

    C. hist_eq.cpp
        hist_eq 함수
            s = T(r) = (L-1)*CDF(r)

    D. hist_matching.cpp
        hist_match 함수
            s = T(r), s = G(z)
            -> z = G^-1(s) -> z = G^-1(T(r))

4. 기본 매개변수 정의
    #define L 256 // 8비트 이미지의 강도 레벨 수
    #define IM_TYPE CV_8UC3 // 기본 이미지 타입이 3채널의 8비트 컬러