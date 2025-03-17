#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize() { 
	dxCommon_ = KamataEngine::DirectXCommon::GetInstance(); 
	input_ = KamataEngine::Input::GetInstance();
	audio_ = KamataEngine::Audio::GetInstance();
}

void TitleScene::Update() { 
	input_->GetJoystickState(0, state); 
	input_->GetJoystickStatePrevious(0, preState);

	if (input_->TriggerKey(DIK_SPACE) || (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(preState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {}
