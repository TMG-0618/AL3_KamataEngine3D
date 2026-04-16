#include "Enemy.h"
#include "MyMath.h"
#include <assert.h>
#include <numbers>

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

	walkTimer_ += 1.0f / 60.0f;

	worldTransform_.translation_ = MyMath::Add(worldTransform_.translation_, velocity_);

	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / 2.0f);
	float degree = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = degree / 180.0f * std::numbers::pi_v<float>;

	// ワールド行列の更新
	worldTransform_.matWorld_ = MyMath::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Enemy::Draw() {
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

void Enemy::OnCollision(const Player* player) { (void)player; }