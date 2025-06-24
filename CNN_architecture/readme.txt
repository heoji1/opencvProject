<CNN architecture for CIFAR-10>

A. 코드 목적: Pytorch를 사용하여 CIFAR-10 데이터 세트에 대한 이미지 분류 작업을 수행하는 CNN 모델을 구현하는 코드이다. VGG16 또는 ResNet50 구조를 기반으로 학습된 CNN 모델을 CIFAR-10 데이터 세트에 대해 불러오고, 이를 CPU(또는 GPU)에서 설정한 epoch동안 학습한 뒤, 텍스트 정확도를 측정한다.

B. 코드 흐름
 B-1. main.py
	1) 라이브러리 및 모델 불러오기: torch, torchvision 등의 Python 기본 라이브러리와 VGG16(또는 ResNet50)모델을 불러온다.
	2) 데이터 전처리 및 로드: 학습/검증 이미지에 정규화 등의 전처리를 적용하고, CIFAR-10 데이터 세트를 로드한다.
	3) 모델 선택 및 가중치 로드: VGG16(또는 ResNet50)모델을 생성하고, 저장된 checkpoint로부터 가중치를 불러온다.
	4) 학습 설정: CrossEntropLoss 손실 함수와 Adam 옵티마이저를 사용하여 학습을 설정한다
	5) 모델 학습: 매 epoch마다 batch 단위로 forward pass -> loss 계산 -> backward pass -> 가중치 업데이트 순으로 모델을 학습하고, 중간 손실을 출력한다.
	6) 테스트 데이터를 통한 정확도 평가: 모델이 테스트 데이터에서 예측한 결과를 바탕으로 전체 정확도를 출력한다.

 B-2. vgg16_full.py
	1) VGG 클래스 정의: features, clasifier로 구성된 VGG 클래스를 정의한다.
	2) features 정의: cfg 리스트에 따른 VGG 구조를 생성한다.
	3) classifier 정의: Dropout, Fully Connected, BatchNorm, ReLU를 포함하는 분류용 계층 시퀀스를 만든다.
	4) 가중치 초기화: 2차원 conv 계층들의 가중치를 He 초기화 방식으로 초기화하고, 편향은 0으로 초기화한다.
	5) forward 함수 정의: 입력을 features에 통과시켜 feature map을 추출하고, 이를 평탄화하여 classifier를 통과시켜 최종 클래스 점수를 출력한다.
	5) vgg16() 함수 정의: cfg 설정에 맞는 VGG16 모델을 생성해 반환한다.

 B-3 resnet50_skeleton.py
	1) 기본 블록 구성 함수 정의: conv1x1, conv3x3 함수를 정의하여 Conv + BatchNorm + ReLU의 기본 블록 조합을 정의한다.
	2) ResidualBlock 클래스 정의:
	  i. 1x1 -> 3x3 -> 1x1 컨볼루션으로 이루어진 bottleneck 블록을 구현한다.
	  ii. downsample 옵션에 따라 입력 크기 및 채널 수를 맞춘다.
	  iii. skip connection을 통해 입력과 출력의 합을 계산한다.
	3) ResNet50_layer4 클래스 정의:
	  i. 초기 Conv(7x7) + MaxPooling 계층으로 입력 이미지를 다운샘플링한다. (layer1)
	  ii. 여러 ResidualBlock 층을 쌓아 네트워크를 구성한다. (layer2~4)
	  iii. average pooling과 fully connected 층으로 CIFAR-10 10개의 클래스를 분류하는 구조를 완성한다.

C. 실행 방법 (MacOS 기준)
	1) python3을 설치한다.
	2) Pytorch와 torchivision을 설치한다. (pip install torch torch vision)
	3) 모델 학습을 위한 vgg16_full.py, resnet50_skeleton.py 파일을 준비한다.
	4) vgg16_full.py, resnet50_skeleton.y 파일을 준비하고 main.py에서 코드를 실행한다.

D. 함수 파라미터 & 기본 파라미터
 D-1. main.py
	* torch.device(device_str)
	  - device_str: cuda나 cpu와 같은 연산에 사용할 디바이스를 지정

	* transforms.Compose([]): 이미지 변환을 순차적으로 묶는 함수
	  - transforms.RandomCrop(size, padding)
		size: 잘라낼 이미지 크기
		padding: 잘라내기 전 이미지에 추가할 패딩 픽셀 수

	  - transforms.RandomHorizontalFlip(): 이미지를 좌우로 무작위 반전
	  - transforms.ToTensor(): 이미지를 PyTorch 텐서로 변환
	  - transforms.Normalize(mean, std)
		mean: 각 채널별 정규화 평균값
		std: 각 채널별 정규화 표준편차

	* torchvision.datasets.CIFAR10
	  - root: 데이터 세트 저장 경로
	  - train: 학습 데이터(True) 또는 검증 데이터(False)
	  - transform: 입력 이미지에 적용할 변환 함수
	  - download: True이면 데이터 세트를 다운로드

	* torch.utils.data.DataLoader
	  - dataset: 사용할 데이터 세트 객체
	  - batch_size: 한 번에 불러올 데이터 샘플의 수
	  - shuffle: True이면 매 epoch마다 데이터 순서를 섞음

	* train 관련 파라미터
	  - num_epochs: 학습 반복 횟수
	  - learning_rate: 옵티마이저의 초기 학습률
	  - criterion: 손실 함수
	  - optimizer: 옵티마이저

	* update_lr
	  - optimizer: 학습에 사용되는 옵티마이저 객체
	  - lr: 새로 적용할 학습률

	* torch.save
	  - path: 저장할 파일 경로

 D-2. vgg16_full.py
	* make_layers
	  - cfg: 레이어의 구성 리스트
	  - batch_norm: 배치 정규화의 적용 여부

	* vgg16(): vgg16 모델의 객체를 반환하는 함수

 D-3. resnet50_skeleton.py
	* conv1x1, conv3x3
	  - in_channels: 입력 채널 수
	  - out_channels: 출력 채널 수
	  - stride: 합성곱의 stride
  	  - padding: 합성곱의 padding 크기

	* ResidualBlock(nn.Module) 클래스
	  - in_channels: 블록의 입력 채널 수
	  - middle_channels: 블록 내부 middle convolution 채널 수
  	  - out_channels: 블록 출력 채널 수
	  - downsample: 다운 샘플링의 여부

	* ResNet50_layer4(nn.Module) 클래스
	  # 초기 함수(__init__)
	    - num_classes: 분류할 클래스 수
	
	  # nn.Conv2d
	    - in_channels: 입력 채널 수
	    - out_channels: 출력 채널 수
	    - kernel_size: 커널 크기
	    - stride: 이동 간격
	    - padding: 패딩 크기

	  # nn.BatchNorm2d
	    - num_features: 입력 채널 수

	  # nn.ReLU
	    - inplace: 메모리 절약을 위한 in-place 연산 여부

	  # nn.MaxPool2d
	    - kernel_size: 풀링 커널 크기
	    - stride: 이동 간격
	    - padding: 패딩 크기

	  # nn.AvgPool2d
	    - kernel_size: 풀링 커널 크기
	    - stride: 이동 간격

	  # nn.Linear
	    - in_features: 입력 차원
	    - out_features: 출력 차원


	  # forward
	    - x: 입력 피처 맵
	
