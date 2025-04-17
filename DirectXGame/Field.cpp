#include "Field.h"
#include "cassert"

Field::~Field() { }

void Field::Initialize(KamataEngine::Model* model) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_.z += 5.0f;
}

void Field::Update() { worldTransform_.UpdateMatrix(); }

void Field::Draw(KamataEngine::Camera& camera) { model_->Draw(worldTransform_, camera); }
