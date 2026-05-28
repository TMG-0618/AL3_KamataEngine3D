#pragma once
#include "AABB.h"
#include "KamataEngine.h"

class Player;
class GameScene;

class ShieldEnemy {

public:
	enum class ShieldLRDirection {
		kRight,
		kLeft,
	};

private:
	enum class Behavior {
		kUnknown,
		kMove,
		kDeath,
		kGuard,
	};

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	static inline const float kWalkSpeed = 0.02f;

	KamataEngine::Vector3 velocity_ = {};

	float baseAngleY_ = 0.0f;

	static inline const float kWalkMotionAngleStart = 0.0f;
	static inline const float kWalkMotionAngleEnd = 90.0f;
	static inline const float kWalkMotionTime = 1.0f;

	float walkTimer_ = 0.0f;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	bool isDead_ = false;

	Behavior behavior_ = Behavior::kMove;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	int deathParameter_ = 0;

	bool isCollisionDisabled_ = false;

	GameScene* gameScene_ = nullptr;

	ShieldLRDirection shieldLRDirection_ = ShieldLRDirection::kLeft;

public:
	ShieldEnemy();
	~ShieldEnemy();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();

	KamataEngine::Vector3 GetWorldPosition();
	AABB GetAABB();

	void OnCollision(Player* player);

	bool IsDead() { return isDead_; }
	bool IsCollisionDisabled() { return isCollisionDisabled_; }

	void BehaviorMoveInitialize();
	void BehaviorDeathInitialize();
	void BehaviorGuardInitialize();

	void BehaviorMoveUpdate();
	void BehaviorDeathUpdate();
	void BehaviorGuardUpdate();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	ShieldLRDirection GetShieldLRDirection() const { return shieldLRDirection_; }

	void SetShieldLRDirection(ShieldLRDirection lrDirection) { shieldLRDirection_ = lrDirection; }
};
