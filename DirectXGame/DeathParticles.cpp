#include "DeathParticles.h"
#include "MyMath.h"
#include<algorithm>

using namespace KamataEngine;

DeathParticles::DeathParticles() {}

DeathParticles::~DeathParticles() {}

void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, Vector3 position) {

	model_ = model;
	camera_ = camera;

	for (WorldTransform& worldTransform : worldTransforms_) {

		worldTransform.Initialize();
		worldTransform.translation_ = position;
		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};
	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

}

void DeathParticles::Update() {

	if (isFinished_) {
		return;
	}

	counter_ += 1.0f / 60.0f;

	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished_ = true;
	}

//loat t = counter_ / kDuration;
	color_.w -= 1.0f / (kDuration*60.0f);

	color_.w = std::clamp(color_.w, 0.0f, 1.0f);

	objectColor_.SetColor(color_);

	for (uint32_t i = 0; i < kNumParticles; ++i) {

		Vector3 velocity = {kSpeed, 0, 0};

		float angle = kAngleUint * i;

		Matrix4x4 matrixRotation = MyMath::MakeRotateZMatrix(angle);
		velocity = MyMath::Transform(velocity, matrixRotation);

		worldTransforms_[i].translation_ = MyMath::Add(worldTransforms_[i].translation_, velocity);
	}

	for (WorldTransform& worldTransform : worldTransforms_) {

		worldTransform.matWorld_ = MyMath::MakeAffinMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);

		worldTransform.TransferMatrix();
	}
}

void DeathParticles::Draw() {

	if (isFinished_) {
		return;
	}
	DebugText::GetInstance()->ConsolePrintf("%f\n",color_.w);
	model_->PreDraw();
	for (WorldTransform& worldTransform : worldTransforms_) {

		model_->Draw(worldTransform, *camera_, &objectColor_);
	}
	model_->PostDraw();
}