#pragma once
#include "3d/Model.h"
#include "3d/WorldTransform.h"
#include "3d/Camera.h"

// ステージの地面
class Field {
public:
	~Field();

	void Initialize(KamataEngine::Model* model);

	void Update();

	void Draw(KamataEngine::Camera& camera);

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;
};