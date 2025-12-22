#include "Skydome.h"

using namespace KamataEngine;

Skydome::Skydome() {}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model, const WorldTransform& worldTransform, Camera* camera) {
	model_ = model;

	worldTransform_.scale_ = worldTransform.scale_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.matWorld_ = worldTransform.matWorld_;
	worldTransform_.parent_ = worldTransform.parent_;

	camera_ = new Camera();

	camera_ = camera;
}

void Skydome::Update() {

	worldTransform_.TransferMatrix();

}

void Skydome::Draw() {

	model_->PreDraw();


	model_->Draw(worldTransform_, *camera_);

	model_->PostDraw();

}