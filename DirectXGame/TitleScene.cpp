#include "TitleScene.h"
#include"MyMath.h"
#include<numbers>
#include<cmath>

using namespace KamataEngine;

TitleScene::TitleScene() {

}

TitleScene::~TitleScene() {

	delete fade_;

}

void TitleScene::Initialize() {

	// カメラ
	camera_ = new Camera();
	camera_->Initialize();

	modelTitle_ = Model::CreateFromOBJ("Title");

	worldTransform_.Initialize();
	worldTransform_.rotation_.x = -std::numbers::pi_v<float> / 2.0f;

	fade_ = new Fade();
	fade_->Initialize();
}

void TitleScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	theta_ += 0.1f;

	if (theta_ > std::numbers::pi_v<float> * 2.0f) {
	
		theta_ -= std::numbers::pi_v<float> * 2.0f;

	}

	fade_->Update();

	worldTransform_.translation_.y = std::sin(theta_) * 7.0f;

	Matrix4x4 affin;

	affin = MyMath::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.matWorld_ = affin;

	worldTransform_.TransferMatrix();
}

void TitleScene::Draw() {

	modelTitle_->PreDraw();

	modelTitle_->Draw(worldTransform_, *camera_);

	modelTitle_->PostDraw();

	fade_->Draw();

}