#pragma once
#include "KamataEngine.h"
#include "Player.h"

class GameScene {

private:

	uint32_t grHandle_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	Player* player_ = nullptr;

public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();
};