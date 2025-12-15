#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() { Initialize(); }

GameScene::~GameScene() { 
	delete sprite_;
}

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("MHbrk.jpg");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
}

void GameScene::Update() {

	Vector2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);

}

void GameScene::Draw() {

	Sprite::PreDraw();

	sprite_->Draw();

	Sprite::PostDraw();

}

