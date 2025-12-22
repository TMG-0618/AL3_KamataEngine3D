#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() { 
	Initialize();
}

GameScene::~GameScene() {
	delete model_;
	delete player_;
}

void GameScene::Initialize() {

	grHandle_ = TextureManager::Load("./Resources/MHbrk.jpg");
	model_ = Model::Create();
	camera_ = new Camera();
	camera_->Initialize();

	player_ = new Player();

	player_->Initialize(model_,grHandle_,camera_);
}

void GameScene::Update() {
	player_->Update();
}

void GameScene::Draw() { 
	player_->Draw(); 
}
