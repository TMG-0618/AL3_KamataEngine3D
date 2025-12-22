#include "Skydome.h"
#include"MyMath.h"

using namespace KamataEngine;

Skydome::Skydome() {}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model, Camera* camera) {
	model_ = model;


	worldTransform_.Initialize();

	worldTransform_.scale_ = {500.0f, 500.0f, 500.0f};

	camera_ = camera;
}

void Skydome::Update() {

	worldTransform_.matWorld_ = MyMath::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

}

void Skydome::Draw() {

	model_->PreDraw();

	model_->Draw(worldTransform_, *camera_);

	model_->PostDraw();

}