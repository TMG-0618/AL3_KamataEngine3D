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
	camera_->translation_ = MyMath::Add(targetWorldTransform.translation_, tagetOffset_);

	camera_->translation_.x = std::clamp(camera_->translation_.x, movableArea_.left, movableArea_.right);
	camera_->translation_.y = std::clamp(camera_->translation_.y, movableArea_.bottom, movableArea_.top);

	camera_->UpdateMatrix();
}

void CameraController::Reset() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	camera_->translation_ = MyMath::Add(targetWorldTransform.translation_, tagetOffset_);
}