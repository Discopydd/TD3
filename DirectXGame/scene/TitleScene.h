#pragma once
#include "audio/Audio.h"
#include "base/DirectXCommon.h"
#include "input/Input.h"
#include "2d/Sprite.h"
#include "3d/Model.h"
#include "3d/Camera.h"
#include "3d/WorldTransform.h"

class TitleScene {
public:
	TitleScene();

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; }

private:
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	/// <summary>
	/// タイトルシーン用
	/// </summary>
	XINPUT_STATE state, preState;

	bool finished_ = false;
};