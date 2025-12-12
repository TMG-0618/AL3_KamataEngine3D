#include "GameScene.h"
#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	//初期化
	KamataEngine::Initialize(L"LC1A_14_タナカ_ミヅキ_AL3");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//ゲームシーン初期化
	GameScene* gameScene = new GameScene();

	//ループ
	while (true) {

		if (KamataEngine::Update()) {
			break;
		};

		// ゲームシーンアップデート
		gameScene->Update();

			//描画開始地点
		dxCommon->PreDraw();

		// ゲームシーン描画
		gameScene->Draw();

		//描画終了地点
		dxCommon->PostDraw();
	}

	//終了
	KamataEngine::Finalize();

	delete gameScene;
	gameScene = nullptr;

	return 0;
}
