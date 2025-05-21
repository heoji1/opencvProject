<Image Segmentation>

개요
Image Segmentation은 Adaptive Threshold, K-means clustering과 Mean shift Algorithm을 활용한 영역별 이미지 분할 기법을 구현하고 있다.
Adapative Threshold 코드는 이미지의 intensity에 따라 0, 255의 single threshold를 이용하고 있다.
K-means clustering, Mean shift Algorithm 코드는 Grayscale, RGB 이미지에 대해 intensity, position을 모두 고려하여 영역별 이미지 분할을 구현하고 있다.

코드 설명
A. Adaptive Thresholding Using Moving Averages
    A-1. 코드 목적: Adaptice Thresholding을 통해 이미지를 비슷한 intensity를 가진 영역별로 분할하기 위한 코드이다.
        Adaptive Thresholding이란?
            영역별 평균 intensity를 고려하여 임계 값(Threshold)를 계산한 후, 임계 값에 따라 이미지 세그먼트를 수행하는 방법이다.

    A-2. 코드 흐름
        1) input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) adaptive_thres 함수를 활용하여 Image segmentation을 수행한다.
        3) 원본 이미지와 결과 이미지를 모두 출력한다.

    A-3. 실행 방법
        함수 내에 input(입력 이미지)와 함께 n(커널의 크기), b(임계값 조정 계수)를 입력한 후 코드를 실행한다.
        ex) output = adaptive_thres(input_gray, 2, 0.9);

    A-4. 함수 매개변수 & 내부 매개변수
        * 함수 매개변수
            n: 커널의 크기 = (2n+1)
            bnumber: 임계값 조정 계수 (평균 intensity에 곱해지는 수)

        * 내부 매개변수
            row, col: 입력 이미지의 행, 열 수
            kernel_size: 커널의 크기
            kernelvalue: uniform filter에서 각 요소가 가지는 가중치 값


B. K-means Clustering
    B-1. 코드 목적: K-means Clustering을 통해 이미지를 비슷한 intensity를 가진 영역별로 분할하기 위한 코드이다.
        K-means Clustering이란?
            이미지 픽셀의 intensity와 position 정보를 이용하여 비슷한 특성을 가진 영역별로 클러스터링을 수행하는 방법이다.

    B-2. 코드 흐름:
        1) input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) 입력 이미지의 intensity, position을 samples에 저장하여 k-means 입력 데이터를 준비한다.
        3) kmeans 함수를 활용하여 K-means Clustering을 수행한다.
        4) 원본 이미지와 결과 이미지를 모두 출력한다.

    B-3. 실행 방법
        함수 내에 이미지 데이터, ㅇㅇ를 입력한 후 코드를 실행한다.
        ex) output = adaptive_thres(input_gray, 2, 0.9);
     
    B-4. 함수 매개변수 & 내부 매개변수
        * 함수 매개변수
            samples: 입력 이미지의 데이터
            clusterCount: 클러스터의 개수
            labels: 각 픽셀에서 클러스터의 인덱스
            criteria: k-means clustering 알고리즘의 종료 조건
            attempts: 알고리즘 시도 횟수
            flags: 초기 클러스터의 중심점 선택 방법
            centers: 클러스터의 중심 좌표

        * 내부 매개변수
            sigma: 상수 요소 (intensity와 position 사이의 비율을 조정하는 상수)
            cluster_idx: 각 데이터 포인트가 속한 클러스터의 인덱스


C. Mean Shift Segmentation
    C-1. 코드 목적: Mean Shift Algorithm을 통해 이미지를 비슷한 intensity를 가진 영역별로 분할하기 위한 코드이다.
        Mean Shift Algorithm이란?
            intensity, position 정보를 모두 고려하여 이미지의 지역극대점(mode)을 찾아 이미지를 분할하는 방법이다.

    C-2. 코드 흐름:
        1) input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) pyrMeanShiftFilteirng 함수를 활용하여 Image Segmentation을 수행한다.
        3) 원본 이미지와 결과 이미지를 모두 출력한다.

    C-3. 실행 방법
        함수 내에 input(입력 이미지), output(결과를 담을 변수), sp(공간 반경), sr(색상 반경), minsize(최소 영역 크기) 코드를 실행한다.
        ex) pyrMeanShiftFiltering(input, output, 31, 20, 3);
     
    C-4. 함수 매개변수
        sp: 공간 반경
        sr: 색상 반경
        minsize: 이미지 피라미드의 레벨