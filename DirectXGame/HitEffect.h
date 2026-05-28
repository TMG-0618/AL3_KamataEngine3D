#pragma once
#include "KamataEngine.h"
#include "Random.h"
class HitEffect {

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
	KamataEngine::WorldTransform circleWorldTransform_;
	std::array<KamataEngine::WorldTransform, 2> ellipseWorldTransform_;
	Random random_;

	Status status_ = Status::kNone;

	float animationParam_ = 0;

	KamataEngine::ObjectColor color_;

public:
	HitEffect() = default;
	~HitEffect() = default;

	static HitEffect* Create(KamataEngine::Vector3 pos);

	void Initialize(KamataEngine::Vector3 pos);

	void Update();

	void Draw();

	static void SetModel(KamataEngine::Model* model) { model_ = model; }
	static void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }

	bool IsDead() const { return status_ == Status::kEnd; }
};
