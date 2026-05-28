#include "GuardEffect.h"
#include "MyMath.h"
#include <numbers>

using namespace KamataEngine;

Model* GuardEffect::model_ = nullptr;
Camera* GuardEffect::camera_ = nullptr;

GuardEffect* GuardEffect::Create(KamataEngine::Vector3 pos) {

	GuardEffect* instance = new GuardEffect();

	assert(instance);

	instance->Initialize(pos);

	return instance;
}

void GuardEffect::Initialize(KamataEngine::Vector3 pos) {

	status_ = Status::kSpread;
	animationParam_ = 0;
	ringWorldTransform_.Initialize();
	ringWorldTransform_.translation_ = pos;
	ringWorldTransform_.scale_ = {0.0f, 0.0f, 1.0f};

	color_.Initialize();
	color_.SetColor({0.8f, 0.8f, 1.0f, 1.0f});
}

void GuardEffect::Update() {

	switch (status_) {

	case Status::kNone:

		break;

	case Status::kSpread:

		animationParam_++;

		ringWorldTransform_.scale_.x = MyMath::EaseOut(0.0f, 1.0f, static_cast<float>(animationParam_) / 20.0f);
		ringWorldTransform_.scale_.y = MyMath::EaseOut(0.0f, 1.0f, static_cast<float>(animationParam_) / 20.0f);


		if (animationParam_ >= 20) {
			animationParam_ = 0;
			status_ = Status::kFadeOut;
		}

		break;
	case Status::kFadeOut:

		animationParam_++;

		color_.SetColor({0.8f, 0.8f, 1.0f, MyMath::EaseIn(1.0f, 0.0f, static_cast<float>(animationParam_) / 20.0f)});

		if (animationParam_ >= 20) {
			animationParam_ = 0;
			status_ = Status::kEnd;
		}

		break;
	case Status::kEnd:

		break;
	}

	ringWorldTransform_.matWorld_ = MyMath::MakeAffinMatrix(ringWorldTransform_.scale_, ringWorldTransform_.rotation_, ringWorldTransform_.translation_);
	ringWorldTransform_.TransferMatrix();
}

void GuardEffect::Draw() {
	model_->PreDraw();

	model_->Draw(ringWorldTransform_, *camera_, &color_);
	model_->PostDraw();
}
