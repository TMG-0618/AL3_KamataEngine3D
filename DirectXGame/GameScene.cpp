#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() { Initialize(); }

GameScene::~GameScene() { 
	delete modelSkydome_;
}

void GameScene::Initialize() { 


	skydome_ = std::make_unique<Skydome>();
	modelSkydome_ = Model::CreateFromOBJ("testSphere_intexture", true);
	skydome_->Initialize();
}

void GameScene::Update() {
	skydome_->Update();

}

void GameScene::Draw() { 
	skydome_->Draw();


}

