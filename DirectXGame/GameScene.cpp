#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;

GameScene::GameScene() { Initialize(); }

GameScene::~GameScene() {
	delete textureModel_;
	delete model_;
	delete modelSkydome_;
	delete modelPlayer_;
	delete modelEnemy_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	for (int32_t i = 0; i < 3; ++i) {
		delete enemies_.front();
		enemies_.pop_front();
	}

	worldTransformBlocks_.clear();

	delete player_;
	delete mapChipField_;
	delete debugCamera_;
	delete camera_;
	delete deathParticles_;
}

void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("uvChecker.png");

	phase_ = Phase::kFadeIn;

	// マップチップ
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// ブロック
	GenerateBlocks();

	// カメラ
	camera_ = new Camera();
	camera_->farZ = 2000.0f;
	camera_->Initialize();

	// プレイヤー
	textureModel_ = Model::Create();

	SpawnPlayer();

	player_->SetMapChipField(mapChipField_);

	// 敵
	modelEnemy_ = Model::CreateFromOBJ("Enemy", true);
	for (int32_t i = 0; i < 3; ++i) {

		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(5 + 5 * i, 18 - 2 * i);
		newEnemy->Initialize(modelEnemy_, camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}
	deathParticles_ = new DeathParticles();
	modelDeathParticles_ = Model::CreateFromOBJ("deathParticle", true);
	deathParticles_->Initialize(modelDeathParticles_, camera_, player_->GetWorldPosition());

	// カメラコントローラー
	cameraController_ = new CameraController();
	cameraController_->Initialize(camera_);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovableArea({10.0f, 90.0f, 5.0f, 15.0f});

	// 天球
	modelSkydome_ = Model::CreateFromOBJ("skydome2", true);
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_, camera_);

	// デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 720);
	debugCamera_->SetFarZ(2000.0f);

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameScene::Update() {
		player_->Update();

}

void GameScene::Draw() {


	player_->Draw();


}

void GameScene::GenerateBlocks() {
	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	const uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	model_ = Model::CreateFromOBJ("block2", true);
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::SpawnPlayer() {

	modelPlayer_ = Model::CreateFromOBJ("player", true);
	player_ = new Player();

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(textureModel_, textureHandle_, camera_);
}

void GameScene::CheckAllCollisions() {

#pragma region 自キャラと敵キャラの当たり判定
	{
		AABB aabb1, aabb2;

		aabb1 = player_->GetAABB();

		for (Enemy* enemy : enemies_) {

			aabb2 = enemy->GetAABB();

			if (AABBCheckCollision(aabb1, aabb2)) {
				player_->OnCollision(enemy);

				enemy->OnCollision(player_);
			}
		}
	}
#pragma endregion
}

bool GameScene::AABBCheckCollision(AABB& aabb1, AABB& aabb2) {

	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}
	return false;
}

void GameScene::ChangePhase() {

	switch (phase_) {

	case Phase::kPlay:

		if (player_->IsDead()) {

			phase_ = Phase::kDeath;

			const Vector3& deathParticlePosition = player_->GetWorldPosition();

			deathParticles_->Initialize(modelDeathParticles_, camera_, deathParticlePosition);
		}

		break;

	case Phase::kDeath:

		break;
	}
}