<Encoder-Decoder>

A. 코드 목적: VOC 2012 데이터 세트에 대한 이미지 분할 작업을 수행하는 CNN 모델을 구현하는 코드이다. U-Net 또는 ResNet-Encoder-UNet 기반의 분할 모델을 학습 및 평가한다.

B. 코드 흐름
	1) 데이터 세트 및 데이터 로더 준비: VOC 2012 데이터 세트를 불러오고, 이미지 전처리를 적용하여 학습/검증용 데이터 로더를 생성한다.
	2) 모델 초기화 및 학습 설정: U-Net 또는 ResNet-Encoder-UNet 모델을 초기화하고, 손실 함수, 옵티마이저, 학습률 스케줄러를 설정한다.
	3) 디바이스 설정: GPU 사용 가능 여부에 따라 모델을 적절한 디바이스로 이동시킨다.
	4) 결과 저장 폴더 생성 및 학습 이력 초기화: 학습 결과와 예측 결과를 저장할 폴더를 생성하고, 에폭별 손실과 정확도를 기록할 딕셔너리를 초기화한다.
	5) 학습 및 검증 루프 실행: 지정한 에폭 수만큼 반복하여, 각 에폭마다 학습(train_model), 학습 데이터 평가(get_loss_train), 검증(val_model)을 수행하고 결과를 출력 및 기록 한다.
	6) 학습 결과 시각화 및 종료: 에폭별 손실과 정확도 변화를 그래프로 그려 저장하고, 학습을 종료한다.

C. 실행 방법 (MacOS 기준)
	1) 데이터 세트 준비: Pascal VOC 2012 데이터 세트를 다운로드 한 후, data_dir 변수에 해당 경로를 지정한다.
	2) 패키지 설치: PyTorch, torchvision 등 필요한 패키지를 설치한다.
	3) 코드 실행: main_skeleton.py 코드를 실행한다.

D. 함수 매개변수 및 기본 매개변수
	D-1. main_skeleton.py
		- batch_size: 한 번에 처리할 데이터 샘플 수
		- learning_rate: 학습 률
		- epochs: 학습을 반복할 횟수

	D-2. modules_skeleton.py
		* train_model 함수
		- trainloader: 학습 데이터를 배치 단위로 불러오는 DataLoader 객체
		- model: 학습할 신경망 모델 객체
		- criterion: 손실 함수 객체
		- optimizer: 모델 파라미터를 업데이트 하기 위한 옵티마이저 객체
		- scheduler: 학습률 스케줄러 객체
		- device: 모델과 데이터를 연삲라 디바이스(cpu or cuda)
		
		* accuracy_check 함수
		- label: 정답 레이블 배열
		- pred: 예측 결고 배열

		* accuracy_check_for_batch 함수
		- labels: 배치 단위의 정답 레이블
		- preds: 배치 단위의 예측 결과
		- batch_size: 배치 크기

		* get_loss_train 함수
		- model: 평가할 신경망 모델
		- trainloader: 학습 데이터 DataLoader
		- criterion: 손실 함수
		- device: 연산 디바이스

		* val_model 함수
		- model: 평가할 신경망 모델
		- valloader: 검증 데이터 DataLoader
		- criterion: 손실 함수
		- device: 연산 디바이스
		- dir: 결과 이미지를 저장하기 위한 디렉토리 경로


	D-3. resnet_encoder_unet_skeleton.py
		* conv1x1, conv3x3 함수
	  	- in_channels: 입력 채널 수
	  	- out_channels: 출력 채널 수
	  	- stride: 합성곱의 stride
  	  	- padding: 합성곱의 padding 크기

		* ResidualBlock(nn.Module) 클래스
		- in_channels: 블록의 입력 채널 수
	  	- middle_channels: 블록 내부 middle convolution 채널 수
  	  	- out_channels: 블록 출력 채널 수
	  	- downsample: 다운 샘플링의 여부

		* conv 함수
		- in_channels: 입력 채널 수
		- out_channels: 출력 채널 수

		* UNetWithResnet50Encoder 클래스
		- n_classes: 분류할 클래스 수 (기본값: 22)

		=> forward 함수
		  - x: 입력 이미지 텐서
		  - with_output_feature_map: 출력 feature map을 반환할지 여부

	D-4. UNet_skeleton.py
		* conv 함수, Uent 클래스
		- in_channels: 입력 채널 수
		- out_channels: 출력 채널 수
		

