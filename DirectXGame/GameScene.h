#pragma once
#include "KamataEngine.h"
#include <vector>

class GameScene {

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera camera_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;

public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();
};