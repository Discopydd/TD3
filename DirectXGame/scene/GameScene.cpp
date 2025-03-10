#include "GameScene.h"
#include "base/TextureManager.h"
#include <cassert>
#include <cmath> 
#include <fstream>
#include "2d/DebugText.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete timer_;
	delete ui_;
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
	 timer_->SetTimeLemit(180.0f);
	 timer_->SetTriggerTime(30.0f);
	 timer_->SetEnemyPwerUpTime(12.0f);

	 ui_ = new PlayUI();
	 ui_->Initialize(HP,input_);
}

void GameScene::Update() { 
	if (input_->TriggerKey(DIK_Q)) {
		timerStart = true;
	}
	if (timerStart) {
    	timer_->Update();
	}

	if (input_->TriggerKey(DIK_D)) {
		HP -= 50;
		ui_->SetCurrentHP(HP);
	}

	if (input_->TriggerKey(DIK_L)) {
		isGetExp = true;
	} 
	if (isGetExp) {
		exp = 100;
		isGetExp = false;
	} else {
		exp = 0;
	}
	ui_->Update(exp);
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
	timer_->Draw();
	ui_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

