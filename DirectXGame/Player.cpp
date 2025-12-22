#include "Player.h"
#include"MyMath.h"
#include <assert.h>

using namespace KamataEngine;

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, Camera* camera) {

	assert(model);

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
}

void Player::Update() {

		worldTransform_.matWorld_ = MyMath::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix(); 
}

void Player::Draw() {
	model_->PreDraw();

	model_->Draw(worldTransform_, *camera_);

	model_->PostDraw();
}