<Region-wise Operation>

개요
Region-wise Operation은 mean filter, gaussian filter, sobel filter, laplacian filter, unsharp masking 총 5개의 이미지 처리 기법을 구현하고 있다.
이 코드들은 Grayscale, RGB 이미지에 대한 필터링 구현을 모두 진행하고 있으며, 경계 처리 방식을 선택할 수 있다.

코드 설명
A. Mean Filter (grayscale & RGB)
    A-1. 코드 목적: 입력된 이미지에 대해 Mean filtering을 적용하기 위한 코드이다.
        Mean Filter란?
            가장 단순한 low-pass filter로, 이미지를 blurring 하는 기법이다.
            커널을 사용하여 주변 픽셀 값의 평균을 계산한 후, 계산된 값을 중심 픽셀에 적용하는 기술이다.

    A-2. 코드 흐름
        1) input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) meanfilter 함수를 활용하여 mean filtering을 적용한다.
            - image boundary process 방식
              	zero-paddle: image boundary 밖의 픽셀 값을 0으로 채워서 경계를 처리하는 방식이다.
                mirroring: 이미지 boundary의 픽셀 값을 mirroring하여 경계를 처리하는 방식이다.
                adjustkernel: boundary 근처의 유효한 픽셀만 고려하여 가중치 합계를 계산한 후 경계를 처리하는 방식이다.
        3) 원본 이미지와 Mean filter 이미지를 모두 출력한다.

    A-3. 실행 방법
        함수 내에 input(입력 이미지)와 함께 n(커널의 수), opt(경계 처리 방식)를 입력한 후 코드를 실행한다.
        ex) output = meanfilter(input_gray, 3, "mirroring");

    A-4. meanfilter 함수 매개변수
        input: 입력 이미지
        n: 커널의 크기
            ex) n=1이면 3x3 커널 (kernel_size = 2*n+1)
        opt: boundary processing 방식을 지정 (zero-paddle, mirroring, adjustkernel)

    A-5. 함수 내부 매개변수
        kernel: mean filter의 가중치를 저장하는 행렬
        row, col: 입력 이미지의 행, 열
        kernel_size: 커널의 크기 = (2*n+1)
        tempa, tempb: mirroring boundary processing 과정에서 사용되는 임시 temp 변수
        kernelvalue: 커널에서의 각 요소가 가지는 가중치 값
	-> Mean filter는 균일한 평균 필터링 기법이기 때문에 모든 kernelvalue가 같다.

B. Gaussian Filter (grayscale & RGB)
    B-1. 코드 목적: 입력된 이미지에 대해 gaussian filtering을 적용하기 위한 코드이다.
        Gaussian Filter란?
            일반적으로 가장 많이 사용되는 이미지 blurring 기법 중 하나로, 가우시안 분포를 따르는 커널을 사용하는 필터링 기법이다.
            Mean Filter보다 더 자연스러운 블러링 결과를 가져온다.

    B-2. 코드 흐름
        1) input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) gaussianfilter 함수를 활용하여 gaussian filtering을 적용한다.
            - boundary process 방식
                zero-paddle
                mirroring
                adjustkernel
        3) 원본 이미지와 gaussian filter 이미지를 모두 출력한다.

    B-3. 실행 방법
	함수 내에 input(입력 이미지)와 함께 n(커널의 수), sigmaS, sigmaT(수직/수평 방향의 표준 편차), opt(경계 처리 방식)을 입력한 후 코드를 실행한다.
    ex) output = gaussianfilter(input_gray, 1,1,1, "zero-paddle");

    B-4. gaussianfilter 함수 매개변수
        input: 입력 이미지
        n: 커널 크기
        sigmaS: 수직 방향의 gaussian 분포의 폭을 결정하는 표준 편차
        sigmaT: 수평 방향의 gaussian 분포의 폭을 결정하는 표준 편차
        opt: boundary processing 방식

    B-5. 함수 내부 매개변수
        kernel: gaussian filter의 가중치를 저장하는 커널 행렬
        row, col: 입력 이미지의 행, 열 수
        kernel_size: 커널의 크기 = (2*n+1)
        tempa, tempb: mirroring boundary processing 과정에서 사용되는 임시 temp 변수
        denom: 커널 normalization에서의 분모 값으로, 모든 커널 값의 합
        kernelvalue: 커널에서의 각 요소가 가지는 가중치 값.

C. Sobel Filter (grayscale & RGB)
    C-1. 코드 목적: 입력된 이미지에 대해 sobel filtering을 적용하기 위한 코드이다.
        Sobel Filter란?
            high-pass filter 기법 중 하나로, 미분을 사용하여 edge 부분을 검출하는 필터링 기법이다.
            수평(x) 및 수직(y) 방향의 기울기를 계산하여 강도의 변화를 검출한다.

    C-2. 코드 흐름
        1) input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) sobelfilter 함수를 활용하여 sobel filtering 적용한다.
		-> mirroring boundary process를 먼저 적용하여 boundary를 처리한 후, 경계 처리 된 이미지에 대해 sobel filtering을 적용한다.
        3) 원본 이미지와 sobel filter 이미지를 모두 출력한다.

    C-3. 실행 방법
	함수 내에 input(입력 이미지)를 입력한 후 코드를 실행한다.
    ex) output = sobelfilter(input_gray);

    C-4. sobelfilter 함수 매개변수
        input: 입력 이미지

    C-5. 함수 내부 매개변수
        kernel: filtering할 커널을 저장하기 위한 matrix
        row, col: 입력 이미지의 행, 열 수
        n: 커널의 크기
        tempa, tempb: mirroring boundary processing 과정에서 사용되는 임시 temp 변수

        Sx: 수평 방향의 커널 가중치
        Sy: 수직 방향의 커널 가중치

        Ix: 수평 방향의 gradient 값
        Iy: 수직 방향의 gradient 값

        M: 수직/수평 방향의 gradient 값을 결합한 gradient 크기

D. Laplacian Filter (grayscale & RGB)
    D-1. 코드 목적: 입력된 이미지에 대해 laplacian filtering을 적용하기 위한 코드이다.
        Laplacian Filter란?
            2차 미분을 사용하여 edge 부분을 검출하는 필터링 기법이다.

    D-2. 코드 흐름
        1) input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) laplacianfilter 함수를 활용하여 laplacian filtering 적용한다.
            -> mirroring boundary process를 먼저 적용하여 경계를 처리한다.
        3) 원본 이미지와 laplacian filter 이미지를 모두 출력한다.

    D-3. 실행 방법
	함수 내에 input(입력 이미지)를 입력한 후 코드를 실행한다.
    ex) output = laplacianfilter(input_gray);

    D-4. laplacianfilter 함수 매개변수
        input: 입력 이미지

    D-5. 함수 내부 매개변수
        kernel:커널 저장 변수
        row, col: 입력 이미지의 행, 열 수
        n: 커널의 크기
        tempa, tempb: 

        L: 2차 미분을 위한 laplacian 커널 가중치

        sum: 각 픽셀에 대한 laplacian filter의 적용 결과를 누적하는 변수


E. Separable Gaussian & Unsharp Masking (grayscale & RGB)
    E-1. 코드 목적: 입력된 이미지에 대해 separable gausssian filtering을 적용한 후, 적용된 이미지를 unsharp masking 하기 위한 코드이다.
        - Unsharp Masking이란?
            고주파 요소를 높여 이미지를 더 선명하게 하는 기법이다
            원본 이미지에서 가우시안 필터를 사용하여 블러링을 수행한 후, 원본 이미지와 블러 이미지의 차이를 계산하여 이미지를 선명하게 하는 기법이다.

        - Separable Gaussian의 구현 방식 차이
            2차원의 가우시안 필터를 수평/수직으로 1차원의 필터 두개로 분리하여 적용하는 방식이다.
            이전의 가우시안 필터 방식은 2D 필터로 O(n^2)인 반면 separable 방식은 O(2*n)이다.

    E-2. 코드 흐름
        1) input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) gaussianfilter_seq 함수를 활용하여 수직 커널과 수평 커널에 대한 분리된 가우시안 필터를 적용한다.
        3) unsharpmask 함수를 활용하여 unsharp masking 적용한다.
        3) 원본 이미지와 unsharp masking 이미지를 모두 출력한다.

    E-3. 실행 방법
	함수 내에 input(입력 이미지)와 함께 n(커널의 수), sigmaS, sigmaT(수직/수평 방향의 표준 편차), opt(경계 처리 방식), k(저주파 요소의 비율)을 입력한 후 코드를 실행한다.
    ex) output = unsharpMask(input_gray, 1,1,1,"mirroring",0.5);

    E-4. gaussianfilter_seq
        1) 함수 매개변수
            input: 입력 이미지
            n: 커널 크기
            sigmaS: 수직 방향의 gaussian 분포의 폭을 결정하는 표준 편차
            sigmaT: 수평 방향의 gaussian 분포의 폭을 결정하는 표준 편차
            opt: boundary processing 방식

        2) 함수 내부 매개변수
            kernel_s: 수직 방향의 1D 가우시안 커널
            kernel_t: 수평 방향의 1D 가우시안 커널

            row,col: 입력 이미지의 행, 열 수
            kernel_size: 커널의 크기 = (2*n+1)
            tempa, tempb: mirroring boundary processing 과정에서 사용되는 임시 temp 변수
            kernelvalue: 커널에서의 각 요소가 가지는 가중치 값.
            denom_s, denom_t:커널 normalization에서의 분모 값으로, 모든 커널 값의 합
            inter_output: 수직 방향에 대한 필터링 후에 중간 결과를 저장하기 위한 행렬
    

    E-5. unsharpmask 함수
        1) 함수 매개변수
            input: 입력 이미지
            n: 커널 크기
            sigmaS, sigmaT: gaussian filter에 전달하기 위한 표준 편차 파라미터
            opt: gaussian filter에 전달하기 위한 boundary processing 방식
            k: 저주파 요소를 줄이기 위한 비율을 결정하는 파라미터

        2) 내부 매개변수
            gaussian: gaussianfilter_seq로 blurring된 이미지
            output: unsharp masking 기법이 적용된 결과 이미지
            I: 원본 이미지의 픽셀 값
            L: 블러 이미지의 픽셀 값
            result: unsharp masking의 공식을 적용한 결과 픽셀 값
            -> result = (I-k*L)(1-k);