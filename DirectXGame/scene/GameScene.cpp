#include "GameScene.h"
#include "base/TextureManager.h"
#include <cassert>
#include <cmath> 
#include <fstream>
#include "2d/DebugText.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model_ = Model::Create();
	camera_.Initialize();


	 debugCamera_ = new DebugCamera(1280, 720);

	 AxisIndicator::GetInstance()->SetVisible(true);

	 AxisIndicator::GetInstance()->SetTargetCamera(&camera_);

	 timer_ = new Timer();
	 timer_->Initialize();
	 timer_->SetTimeLemit(600.0f);
	 timer_->SetTriggerTime(300.0f);

}

void GameScene::Update() { 
	if (input_->TriggerKey(DIK_Q)) {
		timerStart = true;
	}
	if (timerStart) {
    	timer_->Update();
	}


    // 残り時間を分:秒の形式で表示する
	float remainingTime = timer_->GetRemainingTime();
	int minutes = static_cast<int>(remainingTime) / 60; // 分を計算
	int seconds = static_cast<int>(remainingTime) % 60; // 秒を計算

	// "分:秒"の形式に整形して表示
	DebugText::GetInstance()->ConsolePrintf("RemainingTime : %02d:%02d\n", minutes, seconds);


	if (timer_->IsTriggered()) {
    	DebugText::GetInstance()->ConsolePrintf("15 minpassed! Trigger ON!\n");
	}

	if (timer_->IsTimeUp()) {
		DebugText::GetInstance()->ConsolePrintf("Time UP! 30 min passed!\n");
	}
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

