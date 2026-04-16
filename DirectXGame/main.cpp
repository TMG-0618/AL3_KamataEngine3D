#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

enum class Scene {
	kUnknown = 0,
	
	kTitle,
	kGame,
};

Scene scene = Scene::kUnknown;
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;

//プロトタイプ宣言
void ChangeScene();
void UpdateScene();
void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// 初期化
	KamataEngine::Initialize(L"LC1A_14_タナカ_ミヅキ");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	scene = Scene::kTitle;
#ifdef _DEBUG

	scene = Scene::kGame;

#endif
	titleScene = new TitleScene();
	titleScene->Initialize();

	gameScene = new GameScene();

	// ループ
	while (true) {

		if (KamataEngine::Update()) {
			break;
		};

		// ゲームシーンアップデート
		// gameScene->Update();

		// titleScene->Update();

		ChangeScene();

		UpdateScene();

		// 描画開始地点
		dxCommon->PreDraw();

		// ゲームシーン描画
		// gameScene->Draw();

		// titleScene->Draw();

		DrawScene();

		// 描画終了地点
		dxCommon->PostDraw();
	}

	// 終了
	KamataEngine::Finalize();

	delete gameScene;
	delete titleScene;
	gameScene = nullptr;

	return 0;
}

void ChangeScene() {

	switch (scene) {

	case Scene::kTitle:

		if (titleScene->IsFinished()) {

			scene = Scene::kGame;

			delete titleScene;
			titleScene = nullptr;

			gameScene = new GameScene;
		}
		break;
	case Scene::kGame:

		if (gameScene->IsFinished()) {

			scene = Scene::kTitle;

			delete gameScene;
			gameScene = nullptr;

			titleScene = new TitleScene;
			titleScene->Initialize();
		}

		break;
	}
}

void UpdateScene() {

	switch (scene) {

	case Scene::kTitle:

		titleScene->Update();

		break;

	case Scene::kGame:

		gameScene->Update();

		break;
	}
}

void DrawScene() {

	switch (scene) {

	case Scene::kTitle:

		titleScene->Draw();

		break;

	case Scene::kGame:

		gameScene->Draw();

		break;
	}
}