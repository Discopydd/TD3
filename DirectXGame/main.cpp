#include <KamataEngine.h>
#include "scene/GameScene.h"
#include "scene/TitleScene.h"
#include "scene/GameOverScene.h"
using namespace KamataEngine;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
GameOverScene* gameOverScene = nullptr;
// シーン
enum class Scene {
	kUnkown = 0,

	kTitle,
	kGame,
	kOver
};

// 現在のシーン
Scene scene = Scene::kUnkown;

void ChangeScene();

void UpdateScene();

void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();

	titleScene = new TitleScene();
	titleScene->Initialize();

	scene = Scene::kTitle;
#pragma endregion

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// 軸表示の更新
		axisIndicator->Update();
		// シーンの毎フレーム処理
		UpdateScene();
		ChangeScene();
		// ImGui受付終了
		imguiManager->End();
		// 描画開始
		dxCommon->PreDraw();
		// シーンの描画
		DrawScene();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	delete titleScene;
	delete gameScene;

	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

void ChangeScene() {
    switch (scene) {
    case Scene::kTitle:
        if (titleScene->IsFinished()) {
            scene = Scene::kGame;

            delete titleScene;
            titleScene = nullptr;

            gameScene = new GameScene();
            gameScene->Initialize();
        }
        break;
    case Scene::kGame:
        if (gameScene->IsFinished()) {
            scene = Scene::kTitle;

            delete gameScene;
            gameScene = nullptr;

           // gameOverScene = new GameOverScene();
            //gameOverScene->Initialize();

			titleScene = new TitleScene();
			titleScene->Initialize();
        }
        break;
    case Scene::kOver:
        if (gameOverScene->IsFinished()) {
            scene = Scene::kTitle;

            delete gameOverScene;
            gameOverScene = nullptr;

            titleScene = new TitleScene();
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
    case Scene::kOver:
        gameOverScene->Update();
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
		case Scene::kOver:
		gameOverScene->Draw();
		break;
	}
}
