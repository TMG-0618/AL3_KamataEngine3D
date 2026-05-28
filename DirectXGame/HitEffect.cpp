#include "HitEffect.h"
#include "MyMath.h"
#include <numbers>

using namespace KamataEngine;

Model* HitEffect::model_ = nullptr;
Camera* HitEffect::camera_ = nullptr;

HitEffect* HitEffect::Create(KamataEngine::Vector3 pos) {

	HitEffect* instance = new HitEffect();

	assert(instance);

	instance->Initialize(pos);

	return instance;
}

void HitEffect::Initialize(KamataEngine::Vector3 pos) {

	status_ = Status::kSpread;
	animationParam_ = 0;
	circleWorldTransform_.Initialize();
	circleWorldTransform_.translation_ = pos;
	circleWorldTransform_.scale_ = {0.0f, 0.0f, 1.0f};

	color_.Initialize();
	color_.SetColor({1.0f, 0.1f, 0.1f, 1.0f});

	for (WorldTransform& worldTransform : ellipseWorldTransform_) {

		worldTransform.scale_ = {0.0f, 0.0f, 1.0f};
		worldTransform.rotation_ = {0.0f, 0.0f, random_.RandomFloat(-std::numbers::pi_v<float>, std::numbers::pi_v<float>)};
		worldTransform.translation_ = circleWorldTransform_.translation_;

		worldTransform.Initialize();
	}
}

void HitEffect::Update() {

	switch (status_) {

	case Status::kNone:

		break;

	case Status::kSpread:

		animationParam_++;

		circleWorldTransform_.scale_.x = MyMath::EaseOut(0.0f, 1.0f, static_cast<float>(animationParam_) / 20.0f);
		circleWorldTransform_.scale_.y = MyMath::EaseOut(0.0f, 1.0f, static_cast<float>(animationParam_) / 20.0f);


		for (WorldTransform& worldTransform : ellipseWorldTransform_) {

			worldTransform.scale_.x = MyMath::EaseOut(0.0f, 0.1f, static_cast<float>(animationParam_) / 20.0f);
			worldTransform.scale_.y = MyMath::EaseOut(0.0f, 1.7f, static_cast<float>(animationParam_) / 20.0f);
		}

		if (animationParam_ >= 20) {
			animationParam_ = 0;
			status_ = Status::kFadeOut;
		}

		break;
	case Status::kFadeOut:

		animationParam_++;

		color_.SetColor({1.0f, 0.1f, 0.1f, MyMath::EaseIn(1.0f, 0.0f, static_cast<float>(animationParam_) / 20.0f)});

		if (animationParam_ >= 20) {
			animationParam_ = 0;
			status_ = Status::kEnd;
		}

		break;
	case Status::kEnd:

		break;
	}

	circleWorldTransform_.matWorld_ = MyMath::MakeAffinMatrix(circleWorldTransform_.scale_, circleWorldTransform_.rotation_, circleWorldTransform_.translation_);
	circleWorldTransform_.TransferMatrix();

	for (WorldTransform& worldTransform : ellipseWorldTransform_) {

		worldTransform.matWorld_ = MyMath::MakeAffinMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		worldTransform.TransferMatrix();
	}
}

void HitEffect::Draw() {
	model_->PreDraw();

	model_->Draw(circleWorldTransform_, *camera_, &color_);

	for (WorldTransform& worldTransform : ellipseWorldTransform_) {

		model_->Draw(worldTransform, *camera_, &color_);
	}
	model_->PostDraw();
}
