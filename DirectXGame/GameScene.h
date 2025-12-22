#pragma once
#include "KamataEngine.h"

class GameScene {

private:

	int grHandle_ = -1;


public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();
};