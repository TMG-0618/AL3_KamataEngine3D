#pragma once
#include "KamataEngine.h"

enum class LRDirection {
	kRight,
	kLeft,
};

class MapChipField;

class Player {

private:
	struct CollisionMapInfo {
		bool isHitCeiling = false;
		bool isLanding = false;
		bool isHitWall = false;
		KamataEngine::Vector3 moveAmount;
	};

	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner,
	};

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.02f;
	static inline const float kLimitRunSpeed = 0.5f;


	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;
	static inline const float kGravityAcceleration = 2.0f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 0.4f;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	MapChipField* mapChipField_ = nullptr;

	static inline const float kBlank = 0.0001f;

public:
	Player();
	~Player();

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,const KamataEngine::Vector3& position);
	void Update();
	void Draw();

	void Move();

	void SetTranslation(KamataEngine::Vector3 newTranslation) { worldTransform_.translation_ = newTranslation; };
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void CheckHitMap(CollisionMapInfo& info);

	void CheckHitTop(CollisionMapInfo& info);
	void CheckHitBottom(CollisionMapInfo& info);
	void CheckHitRight(CollisionMapInfo& info);
	void CheckHitLeft(CollisionMapInfo& info);

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

	void ResolveMovement(const CollisionMapInfo& info);

	void ResolveCeilingCollision(const CollisionMapInfo& info);

	KamataEngine::WorldTransform& GetWorldTransform() { return worldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }
};
