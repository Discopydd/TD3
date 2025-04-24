#pragma once
#include "audio/Audio.h"
#include "base/DirectXCommon.h"
#include "input/Input.h"
#include "2d/Sprite.h"
#include "3d/Model.h"
#include "3d/Camera.h"
#include "3d/WorldTransform.h"
#include "../Field.h"

// 操作説明シーン
class ExplainScene {
public:
	ExplainScene();

	~ExplainScene();

	void Initialize();

	void Updata();

	void Draw();

	bool IsFinished() const { return isFinished_; }

private:
	KamataEngine::DirectXCommon* dxCommon_ = nullptr;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;

	/// <summary>
	/// 操作説明シーン用
	/// </summary>
	KamataEngine::Camera camera_;
	bool isFinished_ = false;

	KamataEngine::Sprite* controlSprite = nullptr;
	KamataEngine::Sprite* cursorSprite = nullptr;

	uint32_t controlTexture = 0;
	uint32_t cursorTexture = 0;

	uint32_t startSEDatahandle_ = 0;
	uint32_t startSEVoiceHandle_ = 0;

	KamataEngine::Vector2 pos = {0.0f, 0.0f};

	KamataEngine::Model* modelField_ = nullptr;
	Field* field_ = nullptr;
};