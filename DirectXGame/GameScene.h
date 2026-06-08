#pragma once
#include "AABB.h"
#include "CameraController.h"
#include "DeathParticles.h"
#include "Fade.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include"BaseEnemy.h"
#include"BaseEffect.h"
#include <memory>
#include <vector>


enum class Phase {
	kFadeIn,
	kPlay,
	kDeath,
	kFadeOut,
};

class GameScene final {

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	std::unique_ptr<Skydome> skydome_;
	KamataEngine::Model* modelSkydome_ = nullptr;

	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelAttack_ = nullptr;

	MapChipField* mapChipField_;
	CameraController* cameraController_ = nullptr;

	std::list<BaseEnemy*> enemies_;

	KamataEngine::Model* modelEnemy_ = nullptr;

	KamataEngine::Model* modelShieldEnemy_ = nullptr;

	DeathParticles* deathParticles_ = nullptr;
	KamataEngine::Model* modelDeathParticles_ = nullptr;

	Phase phase_;

	Fade* fade_ = nullptr;

	bool finished_ = false;

	std::list<BaseEffect*> effects_;
	KamataEngine::Model* modelHitEffect_ = nullptr;
	KamataEngine::Model* modelGuardEffect_ = nullptr;

	bool reloadRequested_ = false;


public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

	void GenerateFieldObjects();

	void CheckAllCollisions();
	bool AABBCheckCollision(AABB& aabb1, AABB& aabb2);

	void ChangePhase();

	bool IsFinished() const { return finished_; }

	void CreateHitEffect(KamataEngine::Vector3 pos);
	void CreateGuardEffect(KamataEngine::Vector3 pos);

	bool IsReloadRequested() const { return reloadRequested_; }
};

