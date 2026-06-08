#pragma once
#include "AABB.h"
#include "KamataEngine.h"
#include"BaseEnemy.h"

class Player;
class GameScene;

class Enemy final : public BaseEnemy {

private:

	enum class Behavior {
		kUnknown,
		kMove,
		kDeath,
	};

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	static inline const float kWalkSpeed = 0.02f;

	KamataEngine::Vector3 velocity_ = {};

	static inline const float kWalkMotionAngleStart = 0.0f;
	static inline const float kWalkMotionAngleEnd = 15.0f;
	static inline const float kWalkMotionTime = 1.0f;

	float walkTimer_ = 0.0f;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	bool isDead_ = false;

	Behavior behavior_ = Behavior::kMove;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	int deathParameter_ = 0;

	bool isCollisionDisabled_ = false;

	GameScene* gameScene_ = nullptr;;

public:
	Enemy();
	~Enemy();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) override;
	void Update() override;
	void Draw() override;

	KamataEngine::Vector3 GetWorldPosition();
	AABB GetAABB();

	void OnCollision(Player* player) override;

	bool IsDead() { return isDead_; }
	bool IsCollisionDisabled() { return isCollisionDisabled_; }

	void BehaviorMoveInitialize();
	void BehaviorDeathInitialize();

	void BehaviorMoveUpdate();
	void BehaviorDeathUpdate();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
};
