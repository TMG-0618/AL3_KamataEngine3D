#pragma once
#include "KamataEngine.h"

enum class LRDirection {
	kRight,
	kLeft,
};

class Player {

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.02f;

	static inline const float kLimitRunSpeed = 0.5f;

	uint32_t textureHandle_ = 0u;

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;

	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;

	static inline const float kGravityAcceleration = 2.45f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 0.5f;

public:
	Player();
	~Player();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,const KamataEngine::Vector3& position);
	void Update();
	void Draw();

	void SetTranslation(KamataEngine::Vector3 newTranslation) { worldTransform_.translation_ = newTranslation; };

	KamataEngine::WorldTransform& GetWorldTransform() { return worldTransform_; }
};
