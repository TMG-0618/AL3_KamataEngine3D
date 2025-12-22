#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() { Initialize(); }

GameScene::~GameScene() { 

}

void GameScene::Initialize() {

	grHandle_ = KamataEngine::TextureManager::Load("./Resources/MHbrk.jpg");

}

void GameScene::Update() {



}

void GameScene::Draw() {



}

