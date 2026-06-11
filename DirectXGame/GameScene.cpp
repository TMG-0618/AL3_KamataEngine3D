#include "GameScene.h"
#include "Enemy.h"
#include "GuardEffect.h"
#include "HitEffect.h"
#include "MyMath.h"
#include "ShieldEnemy.h"
#include<imgui.h>
#include"StageManager.h"
#include"GlobalVariables.h"

using namespace KamataEngine;

GameScene::GameScene() {}
GameScene::~GameScene() {

	delete model_;
	delete modelSkydome_;
	delete modelPlayer_;
	delete modelEnemy_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	while (!enemies_.empty()) {
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

void GameScene::Initialize(StageManager* stageManager) {
	stageManager_ = stageManager;
	phase_ = Phase::kFadeIn;

	// マップチップ
	mapChipField_ = new MapChipField;
	const StageData& stageData = stageManager_->GetCurrentStageData();

	std::string stageFileName = "Resources/fields/" + stageData.name + ".csv";
	mapChipField_->LoadMapChipCsv(stageFileName);

	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelAttack_ = Model::CreateFromOBJ("hit_effect", true);

	// カメラ
	camera_ = new Camera();
	camera_->farZ = 2000.0f;
	camera_->Initialize();

	// マップチップからフィールドオブジェクト生成
	GenerateFieldObjects();

	deathParticles_ = new DeathParticles();
	modelDeathParticles_ = Model::CreateFromOBJ("deathParticle", true);
	deathParticles_->Initialize(modelDeathParticles_, camera_, player_->GetWorldPosition());

	// カメラコントローラー
	cameraController_ = new CameraController();
	cameraController_->Initialize(camera_);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovableArea({10.0f, 89.0f, 5.0f, 15.0f});
	cameraController_->SetMode(CameraController::Mode::kFollow);

	player_->SetCameraController(cameraController_);

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

	modelHitEffect_ = Model::CreateFromOBJ("particle", true);

	HitEffect::SetModel(modelHitEffect_);
	HitEffect::SetCamera(camera_);

	for (int i = 0; i < 3; i++) {

		HitEffect* newHitEffect = new HitEffect();
		Vector3 hitEffectPosition = {0.0f, 0.0f, 0.0f};
		newHitEffect->Initialize(hitEffectPosition);

		effects_.push_back(newHitEffect);
	}

	modelGuardEffect_ = Model::CreateFromOBJ("ring", true);

	GuardEffect::SetModel(modelGuardEffect_);
	GuardEffect::SetCamera(camera_);

	for (int i = 0; i < 3; i++) {

		GuardEffect* newGuardEffect = new GuardEffect();
		Vector3 guardEffectPosition = {0.0f, 0.0f, 0.0f};
		newGuardEffect->Initialize(guardEffectPosition);

		effects_.push_back(newGuardEffect);
	}
}

void GameScene::Update() {

#ifdef _DEBUG

	ImGui::Begin("reload");

	if (ImGui::Button("reload")) {
		reloadRequested_ = true;
	}

	ImGui::End();

#endif

	skydome_->Update();
	switch (phase_) {

	case Phase::kFadeIn:

		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kPlay;
			fade_->Stop();
		}

	case Phase::kPlay:

#ifdef _DEBUG

		if (Input::GetInstance()->TriggerKey(DIK_C)) {

			isDebugCameraActive_ = !isDebugCameraActive_;
		}

		if (Input::GetInstance()->TriggerKey(DIK_J)) {
			GlobalVariables::GetInstance()->SaveFile("Player");
		}
#endif

		player_->Update();

		cameraController_->Update();

		if (isDebugCameraActive_) {

			debugCamera_->Update();
			camera_->matView = debugCamera_->GetCamera().matView;
			camera_->matProjection = debugCamera_->GetCamera().matProjection;

			camera_->TransferMatrix();
		} else {
			camera_ = cameraController_->GetCamera();
			camera_->UpdateMatrix();
		}

		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

				if (!worldTransformBlock)
					continue;

				Matrix4x4 affin;

				affin = MyMath::MakeAffinMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

				worldTransformBlock->matWorld_ = affin;

				worldTransformBlock->TransferMatrix();
			}
		}

		CheckAllCollisions();

		break;

	case Phase::kDeath:

		if (deathParticles_) {

			deathParticles_->Update();
		}

		if (isDebugCameraActive_) {

			debugCamera_->Update();
			camera_->matView = debugCamera_->GetCamera().matView;
			camera_->matProjection = debugCamera_->GetCamera().matProjection;

			camera_->TransferMatrix();
		} else {
			camera_ = cameraController_->GetCamera();
			camera_->UpdateMatrix();
		}

		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

				if (!worldTransformBlock)
					continue;

				Matrix4x4 affin;

				affin = MyMath::MakeAffinMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

				worldTransformBlock->matWorld_ = affin;

				worldTransformBlock->TransferMatrix();
			}
		}

		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}

		break;

	case Phase::kFadeOut:

		fade_->Update();

		if (fade_->IsFinished()) {

			finished_ = true;
		}

		break;
	}

	enemies_.remove_if([](BaseEnemy* enemy) {
		if (enemy->IsDead()) {

			delete enemy;
			return true;
		}
		return false;
	});

	for (BaseEnemy* enemy : enemies_) {
		enemy->Update();
	}

	for (BaseEffect* effect : effects_) {
		effect->Update();
	}

	ChangePhase();
}

void GameScene::Draw() {

	skydome_->Draw();
	model_->PreDraw();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			model_->Draw(*worldTransformBlock, *camera_);
		}
	}
	model_->PostDraw();

	player_->Draw();

	for (BaseEnemy* enemy : enemies_) {
		enemy->Draw();
	}

	for (BaseEffect* effect : effects_) {
		effect->Draw();
	}
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	fade_->Draw();
}

void GameScene::GenerateFieldObjects() {
	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	const uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	model_ = Model::CreateFromOBJ("block2", true);
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex(j, i);

			switch (mapChipType) {
			case MapChipType::kBlock:
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				break;
			case MapChipType::kPlayer: {

				player_ = new Player();

				Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(j, i);
				player_->Initialize(modelPlayer_, modelAttack_, camera_, playerPosition);

				player_->SetMapChipField(mapChipField_);

				break;
			}

			case MapChipType::kEnemy: {

				uint8_t subID = mapChipField_->GetMapChipSubIDByIndex(j, i);

				switch (subID) {
				case 0: {

					modelEnemy_ = Model::CreateFromOBJ("Enemy", true);

					Enemy* newEnemy = new Enemy();
					Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(j,i);
					newEnemy->Initialize(modelEnemy_, camera_, enemyPosition);
					newEnemy->SetGameScene(this);
					enemies_.push_back(newEnemy);

					break;
				}

				case 1: {

					modelShieldEnemy_ = Model::CreateFromOBJ("shieldEnemy", true);

					ShieldEnemy* newShieldEnemy = new ShieldEnemy();
					Vector3 shieldEnemyPosition = mapChipField_->GetMapChipPositionByIndex(j,i);
					newShieldEnemy->Initialize(modelShieldEnemy_, camera_, shieldEnemyPosition);
					newShieldEnemy->SetGameScene(this);
					enemies_.push_back(newShieldEnemy);

					break;
				}
				default:

					break;
				}
			}

			default:
				break;
			}
		}
	}
}

void GameScene::CheckAllCollisions() {

#pragma region 自キャラと敵キャラの当たり判定
	{
		AABB aabb1, aabb2;

		aabb1 = player_->GetAABB();

		for (BaseEnemy* enemy : enemies_) {

			if (enemy->IsCollisionDisabled()) {
				continue;
			}

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

	case Phase::kFadeIn:
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

void GameScene::CreateHitEffect(KamataEngine::Vector3 pos) {

	HitEffect* newHitEffect = HitEffect::Create(pos);
	effects_.push_back(newHitEffect);
}

void GameScene::CreateGuardEffect(KamataEngine::Vector3 pos) {

	GuardEffect* newGuardEffect = GuardEffect::Create(pos);
	effects_.push_back(newGuardEffect);
}