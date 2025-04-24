#include "ExplainScene.h"
#include "base/TextureManager.h"

ExplainScene::ExplainScene() {}

ExplainScene::~ExplainScene() {
	delete controlSprite;
	delete cursorSprite;
	delete modelField_;
	delete field_;
	audio_->StopWave(bgmVoiceHandle_);
}

void ExplainScene::Initialize() {
	dxCommon_ = KamataEngine::DirectXCommon::GetInstance();
	input_ = KamataEngine::Input::GetInstance();
	audio_ = KamataEngine::Audio::GetInstance();

	camera_.Initialize();

	controlTexture = KamataEngine::TextureManager::Load("control.png");
	cursorTexture = KamataEngine::TextureManager::Load("cursor.png");

	controlSprite = KamataEngine::Sprite::Create(controlTexture, {0.0f, 0.0f});
	cursorSprite = KamataEngine::Sprite::Create(cursorTexture, {0.0f, 0.0f});

	cursorSprite->SetSize({32.0f, 32.0f});

	startSEDatahandle_ = audio_->LoadWave("Audio/start.wav");
	bgmDataHandle_ = audio_->LoadWave("Audio/titleBgm.wav");
	bgmVoiceHandle_ = audio_->PlayWave(bgmDataHandle_, true, 0.7f);

	modelField_ = KamataEngine::Model::CreateFromOBJ("field", true);
	field_ = new Field();
	field_->Initialize(modelField_);
}

void ExplainScene::Updata() {
	if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0)) {
		audio_->StopWave(bgmVoiceHandle_);
		startSEVoiceHandle_ = audio_->PlayWave(startSEDatahandle_, false, 0.3f);
		isFinished_ = true;
	}

	// マウス位置取得
	KamataEngine::Vector2 mousePos = input_->GetMousePosition();
	pos.x = mousePos.x - 11;
	pos.y = mousePos.y - 10;
	cursorSprite->SetPosition(pos);
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
	controlSprite->Draw();
	cursorSprite->Draw();
	// スプライト描画後処理
	KamataEngine::Sprite::PostDraw();

#pragma endregion
}
