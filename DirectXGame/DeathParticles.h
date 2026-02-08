#pragma once
#include"KamataEngine.h"
#include<numbers>
#include<array>
class Player;

class DeathParticles {

private:

	KamataEngine::Model* model_;
	KamataEngine::Camera* camera_;

	static inline const uint32_t kNumParticles = 8;

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;

	static inline const float kDuration = 2.0f;
	static inline const float kSpeed = 0.06f;
	static inline const float kAngleUint = std::numbers::pi_v<float> / (float)kNumParticles;

	bool isFinished_ = false;
	float counter_ = 0.0f;

	KamataEngine::ObjectColor objectColor_;
	KamataEngine::Vector4 color_;

public:
	DeathParticles();
	~DeathParticles();

	void Initialize( KamataEngine::Model* model, KamataEngine::Camera* camera,KamataEngine::Vector3 position);
	void Update();
	void Draw();


};
