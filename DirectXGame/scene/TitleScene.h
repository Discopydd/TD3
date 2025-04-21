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

	KamataEngine::Sprite* cursorSprite = nullptr;
	KamataEngine::Sprite* titleSprite = nullptr;
	KamataEngine::Sprite* guideSprite = nullptr;
	KamataEngine::Sprite* backSprite = nullptr;

	uint32_t cursorTexture = 0;
	uint32_t titleTexture = 0;
	uint32_t guideTexture = 0;
	uint32_t backTexture = 0;

	KamataEngine::Vector2 pos = {0.0f, 0.0f};

	uint32_t startSEDatahandle_ = 0;
	uint32_t startSEVoiceHandle_ = 0;
};