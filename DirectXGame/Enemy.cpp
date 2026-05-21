#include "Enemy.h"
#include "MyMath.h"
#include <assert.h>
#include <numbers>
#include"Player.h"

using namespace KamataEngine;

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;
}

void Enemy::Update() {

	if (behaviorRequest_ != Behavior::kUnknown) {

		behavior_ = behaviorRequest_;
		switch (behavior_) {
		case Behavior::kMove:
			BehaviorMoveInitialize();
			break;
		case Behavior::kDeath:
			BehaviorDeathInitialize();
			break;
		default:
			break;
		}
		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {
	case Behavior::kMove:
		BehaviorMoveUpdate();
		break;

	case Behavior::kDeath:
		BehaviorDeathUpdate();
		break;
	}


}

void Enemy::Draw() {

	if (isDead_)
		return;

	model_->PreDraw();

	model_->Draw(worldTransform_, *camera_);

	model_->PostDraw();
}

KamataEngine::Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.x - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.x + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) {
	(void)player;
	if (behavior_ == Behavior::kDeath) {
		return;
	}

	if (player->IsAttack()) {

		behaviorRequest_ = Behavior::kDeath;
	}
}

void Enemy::BehaviorMoveInitialize() {
	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;
}

void Enemy::BehaviorDeathInitialize() {
	velocity_ = {0.0f, 0.0f, 0.0f};
	walkTimer_ = 0.0f;
}

void Enemy::BehaviorMoveUpdate() {
	walkTimer_ += 1.0f / 60.0f;

	worldTransform_.translation_ = MyMath::Add(worldTransform_.translation_, velocity_);

	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / 2.0f);
	float degree = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = degree / 180.0f * std::numbers::pi_v<float>;

	// ワールド行列の更新
	worldTransform_.matWorld_ = MyMath::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Enemy::BehaviorDeathUpdate() { 
	deathParameter_++;

	worldTransform_.rotation_.x = MyMath::EaseOut(worldTransform_.rotation_.x, -std::numbers::pi_v<float> / 2.0f, static_cast<float>(deathParameter_) / 60.0f);
	worldTransform_.rotation_.y += 0.3f;

	if (deathParameter_ >= 15) {

		worldTransform_.scale_.x = MyMath::EaseOut(worldTransform_.scale_.x, 0.0f, static_cast<float>(deathParameter_) / 45.0f);
		worldTransform_.scale_.y = MyMath::EaseOut(worldTransform_.scale_.y, 0.0f, static_cast<float>(deathParameter_) / 45.0f);
		worldTransform_.scale_.z = MyMath::EaseOut(worldTransform_.scale_.z, 0.0f, static_cast<float>(deathParameter_) / 45.0f);
	}
		worldTransform_.matWorld_ = MyMath::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	if (deathParameter_ >= 60) {
		isDead_ = true;
	}

	//isDead_ = true;
}