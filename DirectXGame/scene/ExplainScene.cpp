#include "ExplainScene.h"
#include "base/TextureManager.h"
#include "2d/DebugText.h"

ExplainScene::ExplainScene() {}

ExplainScene::~ExplainScene() {
	delete controlSprite;
	delete cursorSprite;
	delete purposeSprite;
	delete modelField_;
	delete field_;
	audio_->StopWave(bgmVoiceHandle_);
}

void ExplainScene::Initialize() {
	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	input_ = KamataEngine::Input::GetInstance();
	audio_ = KamataEngine::Audio::GetInstance();
	win = KamataEngine::WinApp::GetInstance();

	camera_.Initialize();

	controlTexture = KamataEngine::TextureManager::Load("control.png");
	cursorTexture = KamataEngine::TextureManager::Load("cursor.png");
	purposeTexture = KamataEngine::TextureManager::Load("purpose.png");

	controlSprite = KamataEngine::Sprite::Create(controlTexture, {0.0f, 0.0f});
	cursorSprite = KamataEngine::Sprite::Create(cursorTexture, {0.0f, 0.0f});
	purposeSprite = KamataEngine::Sprite::Create(purposeTexture, {0.0f, 0.0f});

	cursorSprite->SetSize({32.0f, 32.0f});

	startSEDatahandle_ = audio_->LoadWave("Audio/start.wav");
	bgmDataHandle_ = audio_->LoadWave("Audio/titleBgm.wav");
	bgmVoiceHandle_ = audio_->PlayWave(bgmDataHandle_, true, 0.7f);

	modelField_ = KamataEngine::Model::CreateFromOBJ("field", true);
	field_ = new Field();
	field_->Initialize(modelField_);
}

void ExplainScene::Updata() {
	bool isInputTriggered = input_->TriggerKey(DIK_SPACE) || (IsMouseInWindow(win->GetHwnd()) && input_->IsTriggerMouse(0));

	if (isInputTriggered && page != 2) {
		page = 2;
		startSEVoiceHandle_ = audio_->PlayWave(startSEDatahandle_, false, 0.3f);
	} else if (isInputTriggered && page == 2) {
		startSEVoiceHandle_ = audio_->PlayWave(startSEDatahandle_, false, 0.3f);
		audio_->StopWave(bgmVoiceHandle_);
		isFinished_ = true;
	}


	// マウス位置取得
	KamataEngine::Vector2 mousePos = input_->GetMousePosition();
	pos.x = mousePos.x - 11;
	pos.y = mousePos.y - 10;
	cursorSprite->SetPosition(pos);

	KamataEngine::DebugText::GetInstance()->ConsolePrintf("%d\n", page);
}

void ExplainScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

		// 3Dオブジェクト描画前処理
	KamataEngine::Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	field_->Draw(camera_);

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	KamataEngine::Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	if (page == 1) {
	    controlSprite->Draw();
	} else if (page >= 2) {
		purposeSprite->Draw();
	}
	cursorSprite->Draw();
	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();

#pragma endregion
}

bool ExplainScene::IsMouseInWindow(HWND hwnd) {
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(hwnd, &mousePos);

	RECT rect;
	GetClientRect(hwnd, &rect);

	return (mousePos.x >= 0 && mousePos.x < rect.right && mousePos.y >= 0 && mousePos.y < rect.bottom);

}
