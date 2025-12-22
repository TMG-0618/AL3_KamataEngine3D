#include "Skydome.h"

using namespace KamataEngine;

Skydome::Skydome() {}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model, Camera* camera) {
	model_ = model;


	worldTransform_.Initialize();

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