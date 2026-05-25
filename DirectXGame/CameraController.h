#pragma once
#include "KamataEngine.h"

class Player;


class CameraController {
public:
	enum class Mode {
	kFollow,
	kForcedScroll,
};

private:
	Player* target_ = nullptr;

	KamataEngine::Vector3 targetOffset_ = {0.0f, 0.0f, -15.0f};
	KamataEngine::Vector3 targetPos_ = {};
	KamataEngine::Camera* camera_ = nullptr;

	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	Mode mode_ = Mode::kFollow;

	Rect movableArea_ = {0.0f, 80.0f, 0.0f, 100.0f};

	static inline const float kInterpolationRate = 0.1f;
	static inline const float kVelocityBias = 15.0f;
	static inline const Rect margin = {-10.0f, 10.0f, -5.0f, 5.0f};

public:
	CameraController();
	~CameraController();
	void Initialize(KamataEngine::Camera* camera);
	void Update();

	void Reset();

	void SetTarget(Player* target) { target_ = target; }
	KamataEngine::Camera* GetCamera() { return camera_; }
	void SetMovableArea(Rect area) { movableArea_ = area; }

	void SetMode(Mode mode) { mode_ = mode; }
	Mode GetMode() const { return mode_; }
	KamataEngine::Vector3 GetTargetOffset() { return targetOffset_; }
};
