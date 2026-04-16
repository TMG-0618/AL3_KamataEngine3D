#define NOMINMAX
#include "CameraController.h"
#include "MyMath.h"
#include "Player.h"
#include <algorithm>

using namespace KamataEngine;

CameraController::CameraController() {};
CameraController::~CameraController() {};

void CameraController::Initialize(KamataEngine::Camera* camera) { camera_ = camera; }

void CameraController::Update() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	targetpos_.x = targetWorldTransform.translation_.x + tagetOffset_.x + kVelocityBias * target_->GetVelocity().x;
	targetpos_.y = targetWorldTransform.translation_.y + tagetOffset_.y;
	targetpos_.z = targetWorldTransform.translation_.z + tagetOffset_.z + kVelocityBias * target_->GetVelocity().z;

	camera_->translation_ = MyMath::Lerp(camera_->translation_, targetpos_, kInterpolationRate);

	camera_->translation_.x = std::max(camera_->translation_.x, targetpos_.x + margin.left);
	camera_->translation_.x = std::min(camera_->translation_.x, targetpos_.x + margin.right);
	camera_->translation_.y = std::max(camera_->translation_.y, targetpos_.y + margin.bottom);
	camera_->translation_.y = std::min(camera_->translation_.y, targetpos_.y + margin.top);

	camera_->translation_.x = std::clamp(camera_->translation_.x, movableArea_.left, movableArea_.right);
	camera_->translation_.y = std::clamp(camera_->translation_.y, movableArea_.bottom, movableArea_.top);

	camera_->UpdateMatrix();
}

void CameraController::Reset() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	camera_->translation_ = MyMath::Add(targetWorldTransform.translation_, tagetOffset_);
}