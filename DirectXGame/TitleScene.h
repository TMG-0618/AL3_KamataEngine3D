#pragma once
#include"KamataEngine.h"
#include"Fade.h"

class TitleScene {

private:

	KamataEngine::Model* modelTitle_ = nullptr;
	KamataEngine::Camera* camera_;
	KamataEngine::WorldTransform worldTransform_;

	float theta_ = 0.0f;

	bool finished_ = false;

	Fade* fade_ = nullptr;

public:

	TitleScene();
	~TitleScene();

	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }

};
