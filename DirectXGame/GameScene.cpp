#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() { Initialize(); }

GameScene::~GameScene() {

	delete model_;
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	const uint32_t kNumBlockHorizontal = 20;

	const float kBlockWidth = 2.0f;

	model_ = Model::Create();

	worldTransformBlocks_.resize(kNumBlockHorizontal);

	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {
	
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}
}

void GameScene::Update() {

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		
		Matrix4x4 affin;
		
	}

}

void GameScene::Draw() {}
