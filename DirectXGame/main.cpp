#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	KamataEngine::Initialize(L"LC1A_14_タナカ_ミヅキ_AL3");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();


	// 初期化
	while (true) {

		if (KamataEngine::Update()) {
			break;
		};

		dxCommon->PreDraw();


		dxCommon->PostDraw();
	}

	KamataEngine::Finalize();

	return 0;
}
