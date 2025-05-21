<Descriptor>

개요
Descriptor는 OpenCV의 SIFT 알고리즘을 활용하여 두 이미지 간의 특징점을 추출하고, 이 특징점들의 Descriptor를 이용하여 이미지 간 매칭을 수행하는 프로젝트이다.

코드 설명
A. SIFT(Scale-Invariant Feature Transform)
    A-1. 코드 목적: SIFT 알고리즘을 이용하여 특징점과 Descriptor를 추출하고, 최근접 이웃(Nearest Neighbor) 매칭, Cross-checking, Ratio Test의 매칭 필터링 방식을 통해 정확한 매칭 쌍을 시각화하기 위한 코드이다.
        SIFT의 주요 기능
        - 크기 및 회전에 불변하는 특징점을 추출
        - 최근접 이웃(NN)매칭과 Ratio test + Cross-Check 필터링을 통한 3중 필터링

    A-2. 코드 흐름
        1) 이미지 입력: input(입력 이미지) 변수에 이미지 주소를 입력한다.
        2) SIFT 객체 생성: sift = SIFT::create() 파라미터 설정으로 SIFT 검출기를 생성한다.
        3) 특징점 및 Descriptor 추출: sift->detectAndCompute 함수를 활용하여 특징점을 추출한다.
        4) 특징점 매칭: findPair 함수를 활용하여 특징점을 매칭한다.
        5) 결과 시각화: 결과를 시각화하여 출력한다.

    A-3. 실행 방법
        1) 입력 이미지 준비
        2) 컴파일 및 실행

    A-4. 함수 매개변수 & 내부 매개변수
        * SIFT 검출기 생성 함수SIFT::create()
            함수 매개변수
                nFeatures: 검출할 최대 특징점 수 (0 = 제한 없음)
                -> 특징점 수가 증가할 수록 약한 특징점이 많이 포함됨
                nOctaveLayers: 이미지 피라미트의 계층 수
                -> 값이 증가할수록 스케일 공간 분석의 정밀도가 높아져, 작은 스케일 변화도 잘 감지하게 됨
                contrastThreshold: 낮은 대비의 특징점 필터링 임계 값
                -> 값이 증가할수록 검출되는 특징점 수가 적어지고, 정확성이 높아짐
                edgeThreshold: 에지 응답 임계 값
                -> 값이 증가할수록 에지 특징점의 보존성이 높아져 매칭의 오류 가능성이 높아짐.
                sigma: 초기 이미지의 스무딩 정도를 나타내는 표준편차
                -> 값이 증가할수록 노이즈 제거 효과가 증가함



        *detectAndCompute: 
            함수 매개변수
                input1_gray: grayscale의 입력 이미지
                noArray(): 마스크를 사용하지 않음을 나타내는 빈 행렬
                keypoints1: 검출된 특징점 저장 벡터
                descriptors1: 128차원의 SIFT descriptor 행렬 (각 행이 하나의 특징점에 대응)

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