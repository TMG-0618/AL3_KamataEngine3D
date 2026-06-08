#pragma once
#include "KamataEngine.h"
#include "AABB.h"
class Player;

class BaseEnemy {
public:

	virtual ~BaseEnemy() = default;

	virtual void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
		model;
		camera;
		position;
	};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void OnCollision(Player* player) {
		player;
	};
	virtual AABB GetAABB() { return AABB(); };
	virtual bool IsCollisionDisabled() { return false; };
	virtual bool IsDead() { return false; };
};