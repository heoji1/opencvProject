<Edge and Corner Detection>

개요
Edge and Corner Detection은 이미지의 경계(edge)와 모서리(corner)와 같은 특징점을 검출하는 기법을 구현하는 프로젝트이다.
Laplacian of Gaussian, Canny Edge Detector 기법을 통해 경계를 검출하고,
Harris Corner Detector 기법을 통해 모서리를 검출한다.

코드설명
A. Laplacian of Gaussian
    A-1. 코드 목적: Gaussian 기법을 활용하여 이미지의 노이즈를 제거한 후, Laplacian 기법을 통해 경계를 검출하기 위한 코드이다.

    A-2. 코드 흐름
        1) input(입력 이미지) 병수에 이미지 주소를 입력한다.
        2) Gaussianfilter 함수를 활용하여 Gaussian smoothing을 적용한다.
        3) Laplacianfilter 함수를 활용하여 경계를 강조한다.
        4) 결과 이미지를 출력한다.

    A-3. 실행 방법
        Gaussianfilter 함수 내에 입력 이미지(input_gray), 윈도우 크기(window_radius), 표준편차(sigma_s, sigma_t)를 입력하여 h_f를 계산한다.
        이를 Laplacianfilter 함수 내에 입력하여 라플라시안 필터를 계산한다.
        normalize 함수를 통해 라플라시안 필터 결과를 정규화하여 코드를 실행한다.
        ex) Mat h_f = Gaussianfilter(input_gray, window_radius, sigma_t, sigma_s);	// h(x,y) * f(x,y)
            Mat Laplacian = Laplacianfilter(h_f);
            normalize(Laplacian, Laplacian, 0, 1, NORM_MINMAX);

    A-4. 함수 매개변수 & 내부 매개변수
        * Gaussianfilter
            함수 매개변수
                input: 입력 이미지
                n: 커널의 수
                sigma_t: 수직 방향의 공간적 표준편차
                sigma_s: 수평 방향의 공간적 표준편차

            내부 매개변수
                row, col: 입력 이미지의 행, 열 수
                kernel: get_Gaussian_Kernel을 통해 얻은 커널 가중치 행렬
                input_mirror: Mirroring을 통해 얻은 경계 처리된 이미지
                sum: convolution 연산을 통해 얻은 합
            
            * get_Gaussian_Kernel
                함수 매개변수
                    n: 필터 크기
                    sigma_t: 수직 방향의 공간적 표준편차
                    sigma_s: 수평 방향의 공간적 표준편차
                    normalize: 정규화 여부
                
                내부 매개변수
                    kernel_size: 커널의 크기
                    kernel_sum: 커널 정규화를 위한 분모 값으로, 모든 커널 값의 합


            * Mirroring
                함수 매개변수
                    input: 입력 이미지
                    n: 커널의 수
                
                내부 매개변수
                    row, col: 입력 이미지의 행, 열 수
                    input2: mirroring 방식을 통해 경계 처리된 결과 이미지



        *Laplacianfilter
            함수 매개변수
                input: Gaussian smoothing이 적용된 이미지
            
            내부 매개변수
                row, col: 입력 이미지의 행, 열 수
                kernel: get_Laplacian_Kernel을 통해 얻은 커널 가중치 행렬
                input_mirror: Mirroring을 통해 얻은 경계 처리된 이미지
                sum: convolution 연산을 통해 얻은 합

            * get_Laplacian_Kernel

B. Canny Edge Detector
    B-1. 코드 목적: OpenCV의 Canny() 함수를 활용하여 경계를 검출하는 코드이다.

    B-2. 코드 흐름
        1) input(입력 이미지) 병수에 이미지 주소를 입력한다.
        2) Canny() 함수를 사용해 edge를 검출한다.
        3) 결과 이미지를 출력한다.

    B-3. 실행 방법
        함수 내에 입력 이미지(input), 결과를 담을 변수(output), 임계값(thr1, thr2), 커널 크기(apertureSize), 그래디언트 크기 계산 방식(L2gradient)을 입력하여 코드를 실행한다.
        ex) Canny(input_gray, output, thr1, thr2, apertureSize, false);

    B-4. 함수 매개변수 & 내부 매개변수
        * Canny
            input: 입력 이미지
            output: 결과 이미지
            thr1: 낮은 임계값
            thr2: 높은 임계값
            apertureSize: 소벨 필터의 커널의 크기
            L2gradient: 그래디언트 크기 계산 방식 선택

C. Harris Corner Detector
    C-1. 코드 목적: OpenCV의 cornerHarris() 함수를 활용하여 모서리(corner)를 검출한다.

    C-2. 코드 흐름
        1) input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) 입력 이미지를 grayscale로 변환한다.
        3) corenrHarris() 함수를 사용해 모서리를 검출한다.
        4) 결과 이미지를 출력한다.

    C-3. 실행 방법
        함수 내에 입력 이미지(input_gray), 결과를 담을 변수(output), 주변 커널 크기(blockSize), 소벨 커널 크기(ksize), 경계 처리 방식 선택(borderType)을 입력하여 코드를 실행한다.
        ex) cornerHarris(input_gray, output, blockSize, kSize, k, BORDER_DEFAULT);

    C-4. 함수 매개변수 & 내부 매개변수
        * cornerHarris
            src: 입력 이미지
            dst: 결과 이미지
            blockSize: 주변 커널의 크기
            ksize: sobel 커널의 크기
            k: Harris 강도 계수
            borderType: 경계 처리 방식 선택

        * cornerSubPix
            image: 입력 이미지
            points: 코너 좌표들
            subPixWinSize: 코너 주변에서의 서브 픽셀 윈도우 크기
            zeroZone: 코너 주변에서 평균값 계산을 생략할 중심 영역
            criteria: 알고리즘 종료 조건


        * NonMaximum_Suppression
            함수 매개변수
                output: corner 검출 값을 담은 이미지
                corner_mat: 결과 출력을 위한 0,1 이미지
                radius: NonMaximum_Suppression 수행을 위한 이웃의 크기 반지름

            내부 매개변수
                max: 최댓값 여부 (1: 최댓값, 0: 최댓값 아님)
                center: 현재 위치의 corenr 검출 값


