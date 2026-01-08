#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include "MyMath.h"
#include <algorithm>
#include <assert.h>
#include <numbers>

using namespace KamataEngine;

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {

	assert(model);

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	bool landing = false;

	if (velocity_.y < 0.0f) {

		if (worldTransform_.translation_.y <= 1.0f) {

			landing = true;
		}
	}

	if (onGround_) {

		if (velocity_.y > 0.0f) {

			onGround_ = false;
		}
	} else {

		if (landing) {

			worldTransform_.translation_.y = 1.0f;

			velocity_.x *= (1.0f - kAttenuation);

			velocity_.y = 0.0f;

			onGround_ = true;
		}
	}

	Move();

	CollisionMapInfo collisionMapInfo;

	collisionMapInfo.moveAmount = velocity_;

	CheckHitMap(collisionMapInfo);

	ResolveMovement(collisionMapInfo);

	ResolveCeilingCollision(collisionMapInfo);

	// 旋回制御
	if (turnTimer_ > 0.0f) {

		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		float t = turnTimer_ / kTimeTurn;

		worldTransform_.rotation_.y = destinationRotationY * (1.0f - t) + turnFirstRotationY_ * t;
	}

	worldTransform_.matWorld_ = MyMath::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	model_->PreDraw();

	model_->Draw(worldTransform_, *camera_);

	model_->PostDraw();
}

void Player::Move() {
	if (onGround_) {

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {

					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x < 0.0f) {

					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}

			velocity_.x += acceleration.x;
			velocity_.y += acceleration.y;
			velocity_.z += acceleration.z;

			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {

			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {

			velocity_.y += kJumpAcceleration;
		}

	} else {

		velocity_.y += -kGravityAcceleration / 60.0f;

		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::CheckHitMap(CollisionMapInfo& info) {
	CheckHitTop(info);
	CheckHitBottom(info);
	CheckHitRight(info);
	CheckHitLeft(info);
}

void Player::CheckHitTop(CollisionMapInfo& info) {

	if (info.moveAmount.y <= 0.0f) {

		return;
	}

	std::array<Vector3, static_cast<uint32_t>(kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = CornerPosition(MyMath::Add(worldTransform_.translation_, info.moveAmount), static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	MapChipField::IndexSet indexSet;

	indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hit = true;
	}

	indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hit = true;
	}

	if (hit) {

		indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kRightTop]);

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveAmount.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));

		info.isHitCeiling = true;
	}
}

void Player::CheckHitBottom(CollisionMapInfo& info) { info; }

void Player::CheckHitRight(CollisionMapInfo& info) { info; }

void Player::CheckHitLeft(CollisionMapInfo& info) { info; }

Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0},
	};

	return MyMath::Add(center, offsetTable[static_cast<uint32_t>(corner)]);
}

void Player::ResolveMovement(const CollisionMapInfo& info) { worldTransform_.translation_ = MyMath::Add(worldTransform_.translation_, info.moveAmount); }

void Player::ResolveCeilingCollision(const CollisionMapInfo& info) {

	if (info.isHitCeiling) {

		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0.0f;
	}
}