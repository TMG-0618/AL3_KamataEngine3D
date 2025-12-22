#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include"Player.h"
#include <memory>
#include <vector>

class GameScene {

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

	std::unique_ptr<Skydome> skydome_;
	KamataEngine::Model* modelSkydome_ = nullptr;
	KamataEngine::WorldTransform worldTransformSkydome_;

	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::WorldTransform worldTransfromPlayer_;

public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();
};