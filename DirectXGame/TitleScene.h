#pragma once
#include "Fade.h"
#include "KamataEngine.h"

class TitleScene {

public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

private:
	KamataEngine::Model* modelTitle_ = nullptr;
	KamataEngine::Camera* camera_;
	KamataEngine::WorldTransform worldTransform_;

	float theta_ = 0.0f;

	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;

public:
	TitleScene();
	~TitleScene();

	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }
};
