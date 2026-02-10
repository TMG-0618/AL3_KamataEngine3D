#pragma once
#include "CameraController.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include"Enemy.h"
#include"AABB.h"
#include"DeathParticles.h"
#include <memory>
#include <vector>

enum class Phase {
	kPlay,
	kDeath,
};

class GameScene {

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

	MapChipField* mapChipField_;
	CameraController* cameraController_ = nullptr;

	std::list<Enemy*> enemies_;
	KamataEngine::Model* modelEnemy_ = nullptr;

	DeathParticles* deathParticles_ = nullptr;
	KamataEngine::Model* modelDeathParticles_ = nullptr;

	Phase phase_;

public:

	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

	void GenerateBlocks();
	void SpawnPlayer();

	void CheckAllCollisions();
	bool AABBCheckCollision(AABB& aabb1,AABB& aabb2);

	void ChangePhase();
};