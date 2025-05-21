<Image Restoration>

개요
Image Restoration은 salt&pepper noise removal, simple average filtering과 bilateral filtering을 활용한 gaussian noise removal 총 3개의 이미지 복구 기법을 구현하고 있다.
이 코드들은 Grayscale, RGB 이미지에 대한 노이즈 생성과 제거 구현을 모두 진행하고 있으며, zero-padding, mirroring, adjusting kernel의 총 3가지 경계 처리 방식을 선택할 수 있다.

코드 설명
A. Salt & Pepper Noise Removal (grayscale & RGB)
    A-1. 코드 목적: 입력된 이미지에 대해 Salt and pepper nosie를 생성하고 median filtering을 이용하여 noise를 제거하기 위한 코드이다.
        Salt&Pepper이란?
            이미지의 픽셀 값이 임의로 0(검정, pepper) 또는 255(흰색, salt)로 바뀌는 noise이다.

    A-2. 코드 흐름
        1) input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) Add_salt_pepper_Noise 함수를 활용하여 noise를 생성한다
        3) Salt_pepper_noise_removal 함수를 활용하여 noise가 생성된 이미지에서 noise를 제거하여 이미지 복구를 구현한다.
        4) 원본 이미지와 noise 생성 이미지, noise 제거 이미지를 모두 출력한다.

    A-3. 실행 방법
        1) noise 생성
            함수 내에 input(입력 이미지)와 함께 ps(salt noise의 밀도), pp(pepper noise의 밀도)를 입력한다.
            ex) Mat noise_Gray = Add_salt_pepper_Noise(input_gray, 0.1f, 0.1f);

        2) noise 제거
            함수 내에 input(입력 이미지)와 함께 n(커널의 수), sigma_s, sigm_t(수직/수평 방향의 표준 편차), opt(경계 처리 방식)을 입력한 후 코드를 실행한다.
            ex) Mat Denoised_Gray = Salt_pepper_noise_removal_Gray(noise_Gray, window_radius, "zero-padding");

    A-4. 함수 매개변수 & 내부 매개변수
        1) Add_salt_pepper_Noise
            * 함수 매개변수
                input: 입력 이미지
                ps: salt noise의 density
                pp: pepper noise의 density

            * 함수 내부 매개변수
                output: 결과 이미지
                rng: 난수 생성기 객체
                amount1,amount2: 실제 salt, pepper 개수
                x,y: 무작위의 픽셀 좌표

        2) Salt_pepper_noise_removal (Gray & RGB)
            * 함수 매개변수
                input: 입력 이미지
                n: 윈도우 크기
                opt: boundary processing 방식

            * 함수 내부 매개변수
                row, col: 입력 이미지의 행, 열 수
                kernel_size: 필터 윈도우 크기
                median: sorting한 커널 값의 중간 인덱스
                kernel: 1차원 행렬의 medain filter 커널
                output: 결과 이미지
                index: 윈도우 내 픽셀 저장을 위한 인덱스
                tempa,tempb: mirroring boundary processing 과정에서 사용되는 임시 temp 변수


B. Gaussian Noise Removal with simple average filteirng (grayscale & RGB)
    B-1. 코드 목적: 입력된 이미지에 대해 Gaussian nosie를 생성하고 simple average gaussian filtering을 이용하여 noise를 제거하기 위한 코드이다.
        Gaussian noise란?
            각 픽셀 값에 가우시안 분포를 따라 가중치를 부여하는 noise이다.

    B-2. 코드 흐름
        1) input(입력 이미지) 변수에 이미지 주소를 입력한 후, 64비트의 floating point으로 변환한다.
        2) Add_Gaussian_noise 함수를 활용하여 noise를 생성한다
        3) Gaussianfilter 함수를 활용하여 noise가 생성된 이미지에서 noise를 제거하여 이미지 복구를 구현한다.
        4) 원본 이미지와 noise 생성 이미지, noise 제거 이미지를 모두 출력한다.

    B-3. 실행 방법
        1) noise 생성
            함수 내에 input(입력 이미지)와 함께 mean(평균), sigma(표준편차)를 입력한다.
            ex) Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);

        2) noise 제거
            함수 내에 input(입력 이미지)와 함께 n(커널의 수), sigma_s, sigm_t(수직/수평 방향의 표준 편차), opt(경계 처리 방식)을 입력한 후 코드를 실행한다.
            ex) Mat Denoised_Gray = Gaussianfilter_Gray(noise_Gray, 3, 10, 10, "zero-padding");

    B-4. 함수 매개변수 & 내부 매개변수
        1) Add_Gaussian_noise
            * 함수 매개변수
                input: 입력 이미지
                mean: 평균
                sigma: 표준편차

            * 함수 내부 매개변수
                NoiseArr: 입력 이미지 내에 무작위로 noise를 생성한 이미지
                rng: 난수 생성기 객체
    
        2) Gaussianfilter (Gray & RGB)
            * 함수 매개변수
                input: 입력 이미지
                n: 커널의 수
                sigma_t: 수평 방향의 gaussian 분포의 폭을 결정하는 표준 편차
                sigma_s: 수직 방향의 gaussian 분포의 폭을 결정하는 표준 편차
                opt: boundary processing 방식

            * 함수 내부 매개변수
                row, col: 입력 이미지의 행, 열 수
                kernel_size: 커널의 크기 = (2*n+1)
                tempa,tempb: mirroring boundary processing 과정에서 사용되는 임시 temp 변수
                denom: 커널 normalization에서의 분모 값으로, 모든 커널 값의 합
                kernelvalue: 커널에서의 각 요소가 가지는 가중치 값.
                kernel: gaussian filter의 가중치를 저장하는 커널 행렬


C. Gaussian Noise Removal with bilateral filtering (grayscale & RGB)
    C-1. 코드 목적: 입력된 이미지에 대해 Gaussian nosie를 생성하고 bilateral filtering을 이용하여 noise를 제거하기 위한 코드이다.
        Bilateral filtering이란?
            가장 유명한 filtering 기법 중 하나로, 공간적인 거리와 intensity 거리에 대한 가우시안 함수를 결합하여, 분포를 따라 가중치를 부여하는 noise이다.

    C-2. 코드 흐름
        1) input(입력 이미지) 변수에 이미지 주소를 입력한 후, 64비트의 floating point으로 변환한다.
        2) Add_Gaussian_noise 함수를 활용하여 noise를 생성한다
        3) Bilateralfilter 함수를 활용하여 noise가 생성된 이미지에서 noise를 제거하여 이미지 복구를 구현한다.
        4) 원본 이미지와 noise 생성 이미지, noise 제거 이미지를 모두 출력한다.

    C-3. 실행 방법
        1) noise 생성
            ex) Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);
            
        2) noise 제거
            함수 내에 input(입력 이미지)와 함께 n(커널의 수), sigma_s, sigm_t(수직/수평 방향의 표준 편차), sigma_r(intensity의 표준 편차), opt(경계 처리 방식)을 입력한 후 코드를 실행한다.
            ex) Mat Denoised_Gray = Bilateralfilter_Gray(noise_Gray, 3, 10, 10, 10, "zero-padding");

    C-4. 함수 매개변수 & 내부 매개변수
        1) Add_Gaussian_noise
            * 함수 매개변수
                input: 입력 이미지
                mean: 평균
                sigma: 표준편차

            * 함수 내부 매개변수
                NoiseArr: 입력 이미지 내에 무작위로 noise를 생성한 이미지
                rng: 난수 생성기 객체
                
        2) Bilateralfilter (Gray & RGB)
            * 함수 매개변수
                input: 입력 이미지
                n: 윈도우 크기
                sigma_t: 수직 방향의 공간적 표준편차
                sigma_s: 수평 방향의 공간적 표준편차
                sigma_r: intensity의 표준편차
                opt: boundary processing 방식

            * 함수 내부 매개변수
                row, col: 입력 이미지의 행, 열 수
                kernel_size: 커널의 크기 = (2*n+1)
                tempa,tempb: mirroring boundary processing 과정에서 사용되는 임시 temp 변수
                spatial_dist: x,y 축 공간적 거리에 따른 가중치
                intensity_dist: reference 픽셀과 neighbor 픽셀의 값 차이에 따른 가중치
                kernelvalue: 커널에서의 각 요소가 가지는 최종 가중치 값.