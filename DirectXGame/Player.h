#pragma once
#include "KamataEngine.h"

class Player {

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	uint32_t textureHandle_ = 0u;

public:
	Player();
	~Player();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);
	void Update();
	void Draw();

	void SetTranslation(KamataEngine::Vector3 newTranslation) { worldTransform_.translation_ = newTranslation; };
};
