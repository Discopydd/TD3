#include "Item.h"
#include <iostream>
#include "../scene/GameScene.h"
Item::Item() {}

Item::~Item() {}

void Item::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Item::Update() {
	if (isCollected_)
		return; // 被拾取后不更新

	// 可以加入旋转、浮动等效果
	worldTransform_.rotation_.z += 0.05f; // 让道具缓慢旋转

	worldTransform_.UpdateMatrix();
}

void Item::Draw(KamataEngine::Camera& camera) {
	if (!isCollected_) {
		model_->Draw(worldTransform_, camera);
	}
}

// 获取道具的世界坐标
KamataEngine::Vector3 Item::GetWorldPosition() { return {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]}; }

// 处理拾取逻辑
void Item::Collect() {
	isCollected_ = true;
	
}
