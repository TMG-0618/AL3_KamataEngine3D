#pragma once
#include "KamataEngine.h"
#include <vector>

class GameScene {

private:
	KamataEngine::Model* model_ = nullptr;

	std::vector<KamataEngine::WorldTransform*> worldTransformBlocks_;

public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();
};