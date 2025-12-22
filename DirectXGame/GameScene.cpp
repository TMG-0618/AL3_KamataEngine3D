#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;

GameScene::GameScene() { Initialize(); }

GameScene::~GameScene() {

	delete model_;
	delete modelSkydome_;
	delete modelPlayer_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();

	delete player_;
	delete debugCamera_;
	delete camera_;
}

void GameScene::Initialize() {

	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	model_ = Model::Create();
	camera_ = new Camera();
	camera_->farZ = 2000.0f;
	camera_->Initialize();
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			if ((i + j) % 2 == 0)
				continue;

			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}

	modelPlayer_ = Model::CreateFromOBJ("player2", true);

	player_ = new Player();

	player_->Initialize(modelPlayer_, camera_);

	modelSkydome_ = Model::CreateFromOBJ("skydome2", true);

	worldTransformSkydome_.Initialize();
	skydome_ = std::make_unique<Skydome>();

	skydome_->Initialize(modelSkydome_, camera_);

	debugCamera_ = new DebugCamera(1280, 720);

	debugCamera_->SetFarZ(2000.0f);
}

void GameScene::Update() {

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_C)) {

		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock)
				continue;

			Matrix4x4 affin;

			affin = MyMath::MakeAffinMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			worldTransformBlock->matWorld_ = affin;

			worldTransformBlock->TransferMatrix();
		}
	}

	player_->Update();

	skydome_->Update();

	if (isDebugCameraActive_) {

		debugCamera_->Update();
		camera_->matView = debugCamera_->GetCamera().matView;
		camera_->matProjection = debugCamera_->GetCamera().matProjection;

		camera_->TransferMatrix();
	} else {
		camera_->UpdateMatrix();
	}
}

void GameScene::Draw() {

	model_->PreDraw();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			model_->Draw(*worldTransformBlock, *camera_);
		}
	}

	player_->Draw();

	skydome_->Draw();

	model_->PostDraw();
}
