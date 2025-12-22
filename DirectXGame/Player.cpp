#include "Player.h"
#include <assert.h>

using namespace KamataEngine;

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;
	worldTransform_.Initialize();
}

void Player::Update() { worldTransform_.TransferMatrix(); }

void Player::Draw() {
	model_->PreDraw();

	model_->Draw(worldTransform_, *camera_, textureHandle_);

	model_->PostDraw();
}