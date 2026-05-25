#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include "MyMath.h"
#include <algorithm>
#include <assert.h>
#include <numbers>
#include"CameraController.h"

using namespace KamataEngine;

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, Model* modelAttack, Camera* camera, const Vector3& position) {

	assert(model);

	model_ = model;
	modelAttack_ = modelAttack;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.matWorld_ = MyMath::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	worldTransformAttack_.Initialize();
}

void Player::Update() {

	if (behaviorRequest_ != Behavior::kUnknown) {

		behavior_ = behaviorRequest_;

		switch (behavior_) {

		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;

		case Behavior::kAttack:
			BehaviorAttackInitialize();

			break;
		}

		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {

	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;

	case Behavior::kAttack:

		BehaviorAttackUpdate();
		break;
	}
}

void Player::Draw() {

	if (isDead_)
		return;

	model_->PreDraw();

	model_->Draw(worldTransform_, *camera_);

	model_->PostDraw();

	if (behavior_ == Behavior::kAttack) {

		if (attackPhase_ != AttackPhase::kCharge) {

			modelAttack_->PreDraw();

			modelAttack_->Draw(worldTransformAttack_, *camera_);

			modelAttack_->PostDraw();
		}
	}
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
	MapChipType mapChipTypeNext;
	bool hit = false;

	MapChipField::IndexSet indexSet;

	indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kRightTop]);

		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetmapChipIndexSetByPosition({worldTransform_.translation_.x, worldTransform_.translation_.y + kHeight / 2.0f, worldTransform_.translation_.z});

		if (indexSetNow.yIndex != indexSet.yIndex) {
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.moveAmount.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));

			info.isHitCeiling = true;
		}
	}
}

void Player::CheckHitBottom(CollisionMapInfo& info) {

	if (info.moveAmount.y >= 0.0f) {

		return;
	}

	std::array<Vector3, static_cast<uint32_t>(kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = CornerPosition(MyMath::Add(worldTransform_.translation_, info.moveAmount), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	bool hit = false;

	MapChipField::IndexSet indexSet;

	indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {

		hit = true;
	}

	indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {

		hit = true;
	}

	if (hit) {

		indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kRightBottom]);

		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetmapChipIndexSetByPosition({worldTransform_.translation_.x, worldTransform_.translation_.y - kHeight / 2.0f, worldTransform_.translation_.z});

		if (indexSetNow.yIndex != indexSet.yIndex) {

			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.moveAmount.y = std::max(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));

			info.isLanding = true;
		}
	}
}

void Player::CheckHitRight(CollisionMapInfo& info) {

	if (info.moveAmount.x <= 0.0f) {

		return;
	}

	std::array<Vector3, static_cast<uint32_t>(kNumCorner)> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = CornerPosition(MyMath::Add(worldTransform_.translation_, info.moveAmount), static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	MapChipField::IndexSet indexSet;

	indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hit = true;
	}

	indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hit = true;
	}

	if (hit) {

		indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kRightBottom]);

		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetmapChipIndexSetByPosition({worldTransform_.translation_.x + kWidth / 2.0f, worldTransform_.translation_.y, worldTransform_.translation_.z});

		if (indexSetNow.xIndex != indexSet.xIndex) {

			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.moveAmount.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));

			info.isHitWall = true;
		}
	}
}

void Player::CheckHitLeft(CollisionMapInfo& info) {

	if (info.moveAmount.x >= 0.0f) {

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

	indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hit = true;
	}

	if (hit) {

		indexSet = mapChipField_->GetmapChipIndexSetByPosition(positionsNew[kLeftBottom]);
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetmapChipIndexSetByPosition({worldTransform_.translation_.x - kWidth / 2.0f, worldTransform_.translation_.y, worldTransform_.translation_.z});

		if (indexSetNow.xIndex != indexSet.xIndex) {
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.moveAmount.x = std::max(0.0f, rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));

			info.isHitWall = true;
		}
	}
}

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

void Player::SwitchLandingState(const CollisionMapInfo& info) {

	if (onGround_) {

		if (velocity_.y > 0.0f) {

			onGround_ = false;
		} else {

			std::array<Vector3, static_cast<uint32_t>(kNumCorner)> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {

				positionsNew[i] = CornerPosition(worldTransform_.translation_, static_cast<Corner>(i));
			}

			MapChipType mapChipType;

			bool hit = false;

			MapChipField::IndexSet indexSet;
			float checkOffset = {kBlank + 0.03f};

			indexSet = mapChipField_->GetmapChipIndexSetByPosition(MyMath::Add(positionsNew[kLeftBottom], {0, -checkOffset, 0}));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {

				hit = true;
			}

			indexSet = mapChipField_->GetmapChipIndexSetByPosition(MyMath::Add(positionsNew[kRightBottom], {0, -checkOffset, 0}));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {

				hit = true;
			}

			if (!hit) {

				onGround_ = false;
			}
		}

	} else {

		if (info.isLanding) {
			aerialAttackableAmount = 1;
			onGround_ = true;
			DebugText::GetInstance()->ConsolePrintf("landing\n");
			velocity_.x *= 1.0f - kAttenuationLanding;
			velocity_.y = 0.0f;
		}
	}
}

void Player::ResolveWallCollision(const CollisionMapInfo& info) {

	if (info.isHitWall) {

		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

Vector3 Player::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.x - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.x + kWidth / 2.0f};

	return aabb;
}

void Player::OnCollision(const Enemy* enemy) {

	(void)enemy;

	if (IsAttack()) {

		return;
	}

	// velocity_ = MyMath::Add(velocity_, Vector3({0.0f, 0.0f, 0.0f}));

	isDead_ = true;
}

void Player::BehaviorRootInitialize() {}

void Player::BehaviorAttackInitialize() {
	attackPhase_ = AttackPhase::kCharge;
	attackParameter_ = 0;
	velocity_ = {0.0f, 0.0f, 0.0f};
}

void Player::BehaviorRootUpdate() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		if (aerialAttackableAmount > 0) {

			if (!onGround_) {
				aerialAttackableAmount--;
			}

			behaviorRequest_ = Behavior::kAttack;
		}
	}

	Move();

	CollisionMapInfo collisionMapInfo;

	collisionMapInfo.moveAmount = velocity_;

	CheckHitMap(collisionMapInfo);

	ResolveMovement(collisionMapInfo);

	ResolveCeilingCollision(collisionMapInfo);

	ResolveWallCollision(collisionMapInfo);

	SwitchLandingState(collisionMapInfo);

	// 旋回制御
	if (turnTimer_ > 0.0f) {

		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		float t = turnTimer_ / kTimeTurn;

		worldTransform_.rotation_.y = destinationRotationY * (1.0f - t) + turnFirstRotationY_ * t;
	}

	ClampToScreen();
	worldTransform_.matWorld_ = MyMath::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Player::BehaviorAttackUpdate() {

	attackParameter_++;

	switch (attackPhase_) {

	case AttackPhase::kCharge:
	default: {

		float t = static_cast<float>(attackParameter_) / 15.0f;
		worldTransform_.scale_.z = MyMath::EaseOut(1.0f, 0.3f, t);
		worldTransform_.scale_.y = MyMath::EaseOut(1.0f, 1.6f, t);

		if (attackParameter_ >= 15) {
			attackPhase_ = AttackPhase::kTackle;
			attackParameter_ = 0;
		}
		break;
	}
	case AttackPhase::kTackle: {

		float t = static_cast<float>(attackParameter_) / 10.0f;
		worldTransform_.scale_.z = MyMath::EaseOut(0.3f, 1.3f, t);
		worldTransform_.scale_.z = MyMath::EaseIn(1.6f, 0.7f, t);

		if (attackParameter_ >= 10) {
			attackPhase_ = AttackPhase::kRemaining;
			attackParameter_ = 0;
		}

		break;
	}
	case AttackPhase::kRemaining: {

		float t = static_cast<float>(attackParameter_) / 10.0f;
		worldTransform_.scale_.z = MyMath::EaseOut(1.3f, 1.0f, t);
		worldTransform_.scale_.y = MyMath::EaseOut(0.7f, 1.0f, t);

		if (attackParameter_ >= 10) {

			behaviorRequest_ = Behavior::kRoot;
		}

		break;
	}
	}
	Vector3 velocity{};

	switch (attackPhase_) {

	case AttackPhase::kTackle:

		if (lrDirection_ == LRDirection::kRight) {

			velocity.x = 0.5f;

		} else {
			velocity.x = -0.5f;
		}
		break;
	}

	CollisionMapInfo collisionMapInfo;

	collisionMapInfo.moveAmount = velocity;

	CheckHitMap(collisionMapInfo);

	ResolveMovement(collisionMapInfo);

	ResolveCeilingCollision(collisionMapInfo);

	ResolveWallCollision(collisionMapInfo);

	SwitchLandingState(collisionMapInfo);

	worldTransformAttack_.translation_ = worldTransform_.translation_;
	worldTransformAttack_.rotation_ = worldTransform_.rotation_;

	ClampToScreen();
	worldTransform_.matWorld_ = MyMath::MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransformAttack_.matWorld_ = MyMath::MakeAffinMatrix(worldTransformAttack_.scale_, worldTransformAttack_.rotation_, worldTransformAttack_.translation_);
	worldTransform_.TransferMatrix();
	worldTransformAttack_.TransferMatrix();
}

bool Player::IsAttack() const {

	if (behavior_ == Behavior::kAttack) {
		return true;
	}
	return false;
}

void Player::ClampToScreen() {

	// Followモード中は実行しない
	if (cameraController_->GetMode() != CameraController::Mode::kForcedScroll) {
		return;
	}

	// absで確実に正の値にする
	const float kScreenHalfWidth = std::tan(camera_->fovAngleY / 2.0f) * std::abs(cameraController_->GetTargetOffset().z) * camera_->aspectRatio;

	float cameraLeft = camera_->translation_.x - kScreenHalfWidth;
	float cameraRight = camera_->translation_.x + kScreenHalfWidth;

	bool pushedByScroll = false;

	if (worldTransform_.translation_.x - kWidth / 2.0f < cameraLeft) {
		worldTransform_.translation_.x = cameraLeft + kWidth / 2.0f;
		if (velocity_.x < 0.0f) {
			velocity_.x = 0.0f;
		}
		pushedByScroll = true;
	}

	if (worldTransform_.translation_.x + kWidth / 2.0f > cameraRight) {
		worldTransform_.translation_.x = cameraRight - kWidth / 2.0f;
		if (velocity_.x > 0.0f) {
			velocity_.x = 0.0f;
		}
		pushedByScroll = true;
	}

	if (pushedByScroll) {
		for (uint32_t i = 0; i < kNumCorner; ++i) {
			Vector3 corner = CornerPosition(worldTransform_.translation_, static_cast<Corner>(i));
			MapChipField::IndexSet indexSet = mapChipField_->GetmapChipIndexSetByPosition(corner);
			if (mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock) {
				isDead_ = true;
				break;
			}
		}
	}
}