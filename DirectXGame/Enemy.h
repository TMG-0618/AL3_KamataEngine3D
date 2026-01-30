#pragma once
#include"KamataEngine.h"


class Enemy {

private:

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	static inline const float kWalkSpeed = 0.02f;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kWalkMotionAngleStart = 0.0f;
	static inline const float kWalkMotionAngleEnd = 15.0f;
	static inline const float kWalkMotionTime = 1.0f;

	float walkTimer_ = 0.0f;

public:

	Enemy();
	~Enemy();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();

};
