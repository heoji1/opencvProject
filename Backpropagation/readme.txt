<2-layer Neural Network>

개요
2-layer Neural Network는 2-layer fully-connected neural network를 구현하여 CIFAR-10 데이터 세트에 대한 분류를 수행하기 위한 프로젝트이다.

코드 설명
A. neural_net.py
    A-1. 코드 목적: CIFAR-10 데이터 세트에 대한 분류 작업을 수행하는 Two-Layer fully-connected Neural Network을 구현하기 위한 코드이다.

    A-2. 코드 흐름
        1) 모델 생성
            : TwoLayerNet(input_size, hidden_size, output_size) 인스턴스를 생성한다
        2) 손실(loss) 함수 정의
            : Forward pass를 수행하여 입력 데이터에 대한 클래스별 점수를 계산한다
              softmax + cross-entropy 계산과 L2 Regularization 적용을 통한 손실(loss)과 각 파라미터에 대한 그래디언트(gradients)를 계산한다.
        3) 학습(train) 함수 정의
            : 미니배치 SGD 구현을 통해 가중치를 업데이트 한다.
        4) 예측(predict) 함수 정의
            : 학습된 가중치를 이용하여 입력 데이터의 클래스를 예측한다.

    A-3. 실행 방법 (MacOS)
        1) 3.7.16 버전의 python을 설치한다.
        2) pip install -r requirements.txt 명령어를 통해 파일 내 작성된 필수 라이브러리를 설치한다.
        3) TwoLayerNet 모델을 생성 및 실행한다.

    A-4. 함수 파라미터 & 기본 파라미터
        * __init__
            input_size: 입력 데이터의 차원(D)
            hidden_size: 은닉층 뉴런의 개수(H)
            output_size: 분류할 클래스의 개수(C)
            std: 가중치 초기화 표준편차

        * loss
            X: 입력 데이터 (N x D)
            y: 정답 라벨 (N x 1)
            reg:: L2 Regularization의 강도

        * train
            X,y: 학습 데이터 및 라벨
            X_val, y_val: 검증 데이터 및 라벨
            learning_rate: 학습률
            learning_rate_decay: 학습률 감소 비율
            reg: 정규화 강도
            num_iters: 학습 반복 횟수
            batch_size: 배치 크기
            verbose: 학습 진행상황의 출력 여부

        * predict
            X: 입력 데이터 (N x D)

B. two_layer_net.ipynb
    B-1. 코드 목적: CIFAR-10 데이터 세트에 대한 분류 정확도를 평가하고, 은닉층의 크기, 학습률, 정규화 강도에 따른 정확도를 비교하여 최적화하기 위한 코드이다.

    B-2. 코드 흐름
        1) 데이터 로드 및 전처리
            : 학습/검증/테스트 세트를 분리하고, 픽셀 값을 정규화한다.
        2) 모델 초기화
            : TwoLayerNet 클래스를 생성하여 net 모델을 초기화한다.
        3) 학습 실행 및 손실/정확도 시각화
            : num_iters 만큼 반복하여 SGD를 수행한 뒤, 매 epoch마다 정확도를 출력한다.
                Loss history 그래프와 train/val 정확도를 비교하는 그래프를 출력하여 결과를 시작화한다.
                가중치를 시각화한다.
        4) 하이퍼 파라미터의 그리드 탐색
            : hidden_size, learning_rate, iteration, reg의 하이퍼 파라미터의 변화에 따른 정확도를 비교하여 최상의 모델을 탐색한다.

    B-3. 실행 방법
        1) Jupyter Notebook을 실행한다. 
        2) 실행을 통해 학습/예측을 수행한다.
        
    B-4. 파라미터
        * tuning hyperparameters
            best_acc: 최상의 검증 정확도
            best_stats: 최상의 모델 학습 과정에서의 통계값
            best_params: 최상의 하이퍼파라미터 조합
            results: 모든 실행의 결과
            hidden_sizes: hidden layer의 뉴런 수 후보
            -> hidden size 값이 커질수록 정확도가 높아진다.
            learning_rates: 학습률 후보
            -> learning_rate 값이 커질수록 학습 속도가 빨라진다.
            num_iters: 전체 학습 반복 횟수의 후보
            -> iteration 값이 커질수록 손실을 최소화한다.
            regularization_strengths: L2 정규화 계수 후보
            -> reg 값이 커질수록 과적합을 방지할 수 있다.

            

