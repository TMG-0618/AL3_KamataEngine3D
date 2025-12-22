#pragma once
#include "KamataEngine.h"
#include<memory>
#include"Skydome.h"

class GameScene {

private:

	std::unique_ptr<Skydome> skydome_;

	KamataEngine::Model* modelSkydome_ = nullptr;

public:
	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();
};