#include "GameScene.h"
#include "2d/ImGuiManager.h"

using namespace KamataEngine;

GameScene::GameScene() { Initialize(); }

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("MHbrk.jpg");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	worldTransform_.Initialize();
	camera_.Initialize();
	soundDataHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav");
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
}

void GameScene::Update() {

	debugCamera_->Update();

	Vector2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		Audio::GetInstance()->StopWave(voiceHandle_);
	}
#ifdef _DEBUG
	ImGui::Begin("Debug1");
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("InputFloat3", inputFloat3, 0.0f, 1.0f);

	// ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);
	ImGui::End();

	ImGui::ShowDemoWindow();
#endif

	sprite_->SetPosition(position);
}

void GameScene::Draw() {

	Sprite::PreDraw();

	// sprite_->Draw();

	Sprite::PostDraw();

	Model::PreDraw();

	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);

	Model::PostDraw();

	PrimitiveDrawer::GetInstance()->DrawLine3d({20, 0, 0}, {0, 10, 30}, {1.0f, 0.0f, 0.0f, 1.0f});
}
