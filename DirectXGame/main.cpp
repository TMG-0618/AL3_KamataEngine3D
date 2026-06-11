#include "GameScene.h"
#include "GlobalVariables.h"
#include "KamataEngine.h"
#include "StageManager.h"
#include "TitleScene.h"
#include <Windows.h>
#include <fstream>

using namespace KamataEngine;

enum class Scene {
	kUnknown = 0,

	kTitle,
	kGame,
};

Scene scene = Scene::kUnknown;
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
StageManager* stageManager = nullptr;

// プロトタイプ宣言
void ChangeScene();
void UpdateScene();
void DrawScene();
void LoadDebugSettings();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// 初期化
	KamataEngine::Initialize(L"LC1A_14_タナカ_ミヅキ_AL3");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	stageManager = new StageManager();
	stageManager->LoadStageData("Resources/stageData.csv");

	scene = Scene::kTitle;
#ifdef _DEBUG

	LoadDebugSettings();

	GlobalVariables::GetInstance()->LoadFiles();

	scene = Scene::kGame;
	gameScene = new GameScene();
	gameScene->Initialize(stageManager);

#endif
	titleScene = new TitleScene();
	titleScene->Initialize();

	// ループ
	while (true) {

		if (KamataEngine::Update()) {
			break;
		};

		// ゲームシーンアップデート
		// gameScene->Update();

		// titleScene->Update();

		ChangeScene();

		imguiManager->Begin();
		UpdateScene();
#ifdef _DEBUG
		GlobalVariables::GetInstance()->Update();
#endif
		imguiManager->End();

		// 描画開始地点
		dxCommon->PreDraw();

		// ゲームシーン描画
		// gameScene->Draw();

		// titleScene->Draw();

		DrawScene();

		imguiManager->Draw();

		// 描画終了地点
		dxCommon->PostDraw();
	}

	// 終了
	KamataEngine::Finalize();

	delete gameScene;
	delete titleScene;
	delete stageManager;
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
		} else if (gameScene->IsReloadRequested()) {
			delete gameScene;
			gameScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize(stageManager);
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

void LoadDebugSettings() {
	std::ifstream file;
	file.open("debugSettings.ini");

	assert(file.is_open() && "debugSettings.iniはありません");

	std::stringstream debugSettings;
	debugSettings << file.rdbuf();
	file.close();
	std::string line;

	getline(debugSettings, line);
	std::stringstream lineStream(line);

	std::string word;
	std::getline(lineStream, word, ',');
	std::string key = word;

	std::getline(lineStream, word, ',');
	std::string value = word;

	if (key == "InitialStage") {
		stageManager->SetCurrentStageIndexByName(value);
	}
}