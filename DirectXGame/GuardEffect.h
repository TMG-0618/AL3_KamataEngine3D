#pragma once
#include "KamataEngine.h"
#include "Random.h"
#include"BaseEffect.h"

class GuardEffect final : public BaseEffect {

public:
	enum class Status {

		kNone,
		kSpread,
		kFadeOut,
		kEnd,

	};

private:

	static KamataEngine::Model* model_;
	static KamataEngine::Camera* camera_;
	KamataEngine::WorldTransform ringWorldTransform_;
	Random random_;

	Status status_ = Status::kNone;

	float animationParam_ = 0;

	KamataEngine::ObjectColor color_;

public:
	GuardEffect() = default;
	~GuardEffect() = default;

	static GuardEffect* Create(KamataEngine::Vector3 pos);

	void Initialize(KamataEngine::Vector3 pos);

	void Update() override;

	void Draw() override;

	static void SetModel(KamataEngine::Model* model) { model_ = model; }
	static void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }

	bool IsDead() const { return status_ == Status::kEnd; }
};
