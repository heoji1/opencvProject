<Fitting>

개요
Fitting은 SIFT 알고리즘을 활용한 이미지 특징점 매칭과 Affine 변환을 통해 두 이미지를 stitching 하는 프로젝트이다.

코드 설명
A. Stitching-case1 (Mx = b)
    A-1. 코드 목적
        : 두 이미지를 자동으로 정합(stitching)하여 하나의 파노라마 이미지로 합치는 기능을 구현하기 위한 코드이다. 
        SIFT 알고리즘을 활용하여 이미지 간 특징점을 추출하고, 대응점 매칭, affine 변환 행렬 계산, 역변환 및 블렌딩을 통해 이미지를 자연스럽게 이어붙인다.
    
    A-2. 코드 흐름
        1) 이미지 입력 및 전처리: 두 입력 이미지를 읽고, 크기 조정 및 그레이스케일로의 변환을 수행한다.
        2) SIFT 특징점 추출: SIFT 알고리즘을 활용하여 이미지의 특징점과 디스크립터를 추출한다.
        3) 특징점 매칭: 최근접 이웃을 찾고, ratio test, cross-check 검증을 통해 매칭 쌍을 추출한다.
        4) Affine 변환 행렬 계산: 매칭된 좌표들을 통해 affine 변환 행렬을 계산하여, 두 번째 이미지를 첫 번째 이미지의 좌표계로 변환한다.
        5) 역변환 및 보간: affine 변환 행렬을 이용하여 두 번째 이미지를 첫 번째 이미지 영역으로 역변환한다.
        6) 이미지 합성: 두 이미지가 겹치는 부분을 블랜딩하여 자연스럽게 합성한다.

    A-3. 실행 방법
        1) OpenCV 라이브러리를 설치하고 C++ 컴파일러 등 환경을 준비한다.
        2) 코드와 같은 디렉토리 내에 입력 이미지 2장을 준비한다.
        3) 컴파일 및 실행한다.
    
    A-4. 함수 파라미터 & 기본 파라미터
        * SIFT: 특징점 검출
            nFeatures: 최대 검출 특징점 수 
                -> 0 = 자동 선택
                -> 값이 커질수록 특징점 수 증가

            nOctaveLayers: 스케일 공간 계층 수
                -> 값이 커질수록 세부 스케일 분석 강화

            contrastThreshold: 낮은 대비의 특징점 필터링 임계값
                -> 값이 커질수록 특징점 수 감소

            edgeThreshold: 에지 응답 임계값
                -> 값이 커질수록 경계 영역의 특징점 감소

            sigma: 초기 가우시안 블러의 정도를 나타내는 표준편차
                -> 값이 커질수록 초기 가우시안 스무딩 강화

        * 특징점 매칭 파라미터
            RATIO_THR: ratio thresholding의 비율 임계값
                -> 값이 커질수록 매칭 수가 증가

            ratio_threshold: ratio test 활성화
                -> 활성화 시 정확도가 상승하지만, 매칭 수가 적어짐

            crossCheck: 양방향 검증 활성화
                -> 활성화 시 정확도가 상승하지만, 매칭 수가 적어짐

        * cal_affine: Affine 변환 계산
            ptl_x, ptl_y: src 이미지의 좌표
                -> 최소 3개 이상의 좌표 필요

            ptr_x, ptr_y: dst 이미지의 좌표
                -> 최소 3개 이상의 좌표 필요

            n: 사용된 매칭점 수
                -> 3개 이상의 매칭점 필요

        * blend_stitching: 이미지 블랜딩을 수행한다.
            I1, I2: 블랜딩을 수행할 이미지
            I_f: 최종 stitching 결과 이미지
            bound_l, bound_u: stitching 이미지의 경계 좌표를 계산
            alpha(=0.5): 블랜딩 가중치

        * euclidDistance: 두 특징 벡터의 유클리드 거리를 계산하여 반환한다.
            함수 매개변수
                vec1, vec2: 각 객체들의 한 행에서의 여러 열을 가진 벡터
            
            내부 매개변수
                sum: 최종 거리의 제곱 값
                dim: 벡터의 열의 수 (vec1와 vec2의 열의 수는 같아야 함)

        * nearestNeighbor: 주어진 특징 벡터와 가장 가까운 이웃의 인덱스를 반환한다.
            함수 매개변수
                vec: 비교 대상이 되는 특징 descriptor
                keypoints: 후보 특징점의 목록
                descriptors: keypoints들의 descirptor 집합

            내부 매개변수
                neighbor: 최근접 이웃의 인덱스
                v: i번째 descriptor
                minDist: 최소 거리
                Dist: vec와 v 사이의 유클리드 거리

        * findPairs: crossCheck, ratio_threshold 매칭 필터링 방식을 적용하여 매칭 쌍을 추출한다.
            함수 매개변수
                keypoints1, descriptors1: 소스 이미지의 특징점 데이터
                keypoints2, descriptors2: 타겟 이미지의 특징점 데이터
                srcPoints, dstPoints: 소스/타겟 이미지의 매칭점 좌표
                crossCheck: 양방향 검증 방식의 활용 여부(true/false)
                ratio_threshold: 최선과 차선의 매칭 거리 비율 비교 방식의 활용 여부(true/false)

            내부 매개변수
                * ratio_threshold
                    RATIO_THR: 최적/차선 매칭 간 거리 비율 임계 값
                    -> 낮아질 수록 더 엄격한 매칭이 가능함

                    desc1: 소스 이미지의 현재 descriptor
                    desc2: 타겟 이미지의 nn 인덱스에 해당하는 descriptor
                    Dists: desc1과 타겟 이미지의 모든 descriptor 간의 거리를 저장한 배열
                    dist1, dist2: 최적/차선 거리
                ## ratio 테스트 방식을 진행할 경우 최적/차선 거리 비율에 따른 모호한 매칭을 제거함으로써 정확도를 향상시킬 수 있다.

                * crossCheck
                    nn: 소스->타겟 방향의 최근접 이웃 인덱스
                    nn2: 타겟->소스 방향의 최근접 이웃 인덱스
                    pt1, pt2: 각 이미지에서의 현재 키포인트
                ## cross-check 방식을 진행할 경우 일방향으로만 매칭된 특징점들을 제거함으로써 정확도를 향상시킬 수 있다.


B. Stitching-case2 (Mx = b + RANSAC)
    B-1. 코드 목적
        : 두 이미지를 자동으로 정합(stitching)하여 하나의 파노라마 이미지로 합치는 기능을 구현하기 위한 코드이다. 
        case1 코드의 기능에 더해, 특징점 매칭 시 RANSAC 알고리즘을 이용하여 outlier를 자동으로 제거한 후 affine 변환을 계산한다.

    B-2. 코드 흐름
        1) 이미지 입력 및 전처리: 두 입력 이미지를 읽고, 크기 조정 및 그레이스케일로의 변환을 수행한다.
        2) SIFT 특징점 추출: SIFT 알고리즘을 활용하여 이미지의 특징점과 디스크립터를 추출한다.
        3) 특징점 매칭: 최근접 이웃을 찾고, ratio test, cross-check 검증을 통해 매칭 쌍을 추출한다.
        4) RANSAC 기반의 Affine 변환 행렬 계산: RANSAC 알고리즘을 활용하여 affine 변환 행렬을 계산하여, 두 번째 이미지를 첫 번째 이미지의 좌표계로 변환한다.
        5) 역변환 및 보간: affine 변호나 행렬을 이용하여 두 번째 이미지를 첫 번째 이미지 영역으로 역변환한다.
        6) 이미지 합성: 두 이미지가 겹치는 부분을 블랜딩하여 자연스럽게 합성한다.

    B-3. 실행 방법
        1) OpenCV 라이브러리를 설치하고 C++ 컴파일러 등 환경을 준비한다.
        2) 코드와 같은 디렉토리 내에 입력 이미지 2장을 준비한다.
        3) 컴파일 및 실행한다.

    B-4. 함수 파라미터 & 기본 파라미터
        * ransac_affine: RANSAC 알고리즘을 활용한 Affine 변환 계산
            함수 매개변수
                srcPoints, dstPoints: 소스/타겟 이미지의 매칭점 좌표
                S: 샘플링 반복 횟수
                    -> 값이 커질수록 정확도가 높아짐

                thr: inlier를 판정하는 임계값
                    -> 값이 작아질수록 판정 기준이 엄격해짐

                k: 부분 집합의 크기

            내부 매개변수 
                best_count: 최대 inlier의 수
                count: 현재 inlier의 수
                bestTrans: 최적의 affine 행렬
                Trans: 현재 affine 행렬
                bestInlierFlags: 최적 모델에서 각 매칭점의 inlier 여부
                current_inliers: 현재 모델의 각 매칭점의 inlier 여부
                ptl_x, ptl_y: src 이미지의 좌표
                ptr_x, ptr_y: dst 이미지의 좌표
                idxs: 무작위의 샘플 인덱스
                idx: 현재 동작 중인 인덱스
                only: 중복 샘플링 검사
                Px, Py: 투영된 좌표
                dx, dy: 좌표 간 차이
                dist: 유클리드 오차 거리
                refineTrans: 정제된 affine 행렬

C. Hough Transform
    C-1. 코드 목적
        : 입력 이미지에 대한 직선을 검출하기 위한 코드이다. Canny 에지 검출을 통해 경계를 추출하고, Hough Transform을 통해 직선을 검출한다.

    C-2. 코드 흐름
        1) 이미지 로드: 입력 이미지를  읽어온다.
        2) canny 에지 검출: canny 함수를 활용하여 경계를 검출하여, 직선 검출의 입력으로 사용하기 위한 경계 이미지를 생성한다.
        3) 이미지 변환: 직선을 컬러로 시각화하기 위해 경계 이미지를 컬러 이미지로 변환한다.
        4) 직선 검출: HoughLines/HoughLinesP 함수를 활용하여 직선 검출 정보를 가져온다.
        5) 직선 시각화: 검출된 직선을 이미지 내에 빨간 선으로 시각화한다.
        6) 이미지 표시:  결과 이미지를 출력한다.

    C-3. 실행 방법
        1) OpenCV 라이브러리를 설치하고 C++ 컴파일러 등 환경을 준비한다.
        2) 코드와 같은 디렉토리 내에 입력 이미지를 준비한다.
        3) 컴파일 및 실행한다.

    C-4. 함수 매개변수 & 내부 매개변수
        * HoughLines 함수
            dst: 입력 이미지 (Canny 에지 검출을 통한 에지 이미지)
            lines: 결과 직선 정보 벡터
            rho: 거리의 단위 간격(거리 해상도)
            theta: 각도의 단위 간격(각도 해상도)
            threshold: 투표 누적값의 임계값
            srn: rho 축에서 누적수의 세분화 간격
            stn: theta 축에서 누적수의 세분화 간격

        * HoughLinesP 함수
            dst: 입력 이미지 (Canny 에지 검출을 통한 에지 이미지)
            lines: 결과 직선 정보 벡터
            rho: 거리의 단위 간격
            theta: 각도의 단위 간격
            threshold: 투표 누적값의 임계값
            minLineLength: 최소 직선의 길이
            maxLineGap: 선분 사이의 최대 간격