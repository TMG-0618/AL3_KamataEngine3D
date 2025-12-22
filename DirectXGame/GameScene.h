#pragma once
#include "KamataEngine.h"

class GameScene {

private:

	uint32_t grHandle_ = -1;


public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();
};