#include "TitleScene.h"
#include "base/TextureManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete cursorSprite;
	delete titleSprite;
	delete guideSprite;
	delete backSprite;
}

void TitleScene::Initialize() { 
	dxCommon_ = KamataEngine::DirectXCommon::GetInstance(); 
	input_ = KamataEngine::Input::GetInstance();
	audio_ = KamataEngine::Audio::GetInstance();

	cursorTexture = KamataEngine::TextureManager::Load("cursor.png");
	titleTexture = KamataEngine::TextureManager::Load("title.png");
	guideTexture = KamataEngine::TextureManager::Load("titleGuide.png");
	backTexture = KamataEngine::TextureManager::Load("back.png");

	cursorSprite = KamataEngine::Sprite::Create(cursorTexture, {0.0f, 0.0f});
	titleSprite = KamataEngine::Sprite::Create(titleTexture, {0.0f, 0.0f});
	guideSprite = KamataEngine::Sprite::Create(guideTexture, {-50.0f, 20.0f});
	backSprite = KamataEngine::Sprite::Create(backTexture, {0.0f, 0.0f});

	cursorSprite->SetSize({32.0f, 32.0f});
}

void TitleScene::Update() { 
	input_->GetJoystickState(0, state); 
	input_->GetJoystickStatePrevious(0, preState);

	if (input_->TriggerKey(DIK_SPACE) || (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(preState.Gamepad.wButtons & XINPUT_GAMEPAD_A) || input_->IsTriggerMouse(0)) {
		finished_ = true;
	}

	// マウス位置取得
	KamataEngine::Vector2 mousePos = input_->GetMousePosition();
	pos.x = mousePos.x - 11;
	pos.y = mousePos.y - 10;
	cursorSprite->SetPosition(pos);
}

void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	// タイトル背景（変えてもOK））
	backSprite->Draw();
	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	titleSprite->Draw();
	guideSprite->Draw();
	cursorSprite->Draw();
	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();

#pragma endregion
}
