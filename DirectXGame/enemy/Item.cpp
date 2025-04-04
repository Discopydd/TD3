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
	 worldTransform_.scale_ = { 1.5f,1.5f,1.5f };
	 objectColor_.Initialize();
	color_ = {0, 1, 0, 1};
	 alpha_ = 1.0f;
}

void Item::Update() {
	if (isCollected_){
		  // 拾取后的逻辑：旋转并向上移动
        moveTimer_ += 1.0f / 60.0f;
	 // 旋转效果
        worldTransform_.rotation_.z += 0.2f; // 加快旋转速度

        // 向上移动（Z轴方向）
        worldTransform_.translation_.z -= 0.1f;
		 alpha_ = 1.0f - (moveTimer_ / 0.5f);
        alpha_ = max(0.0f, alpha_);
        // 1秒后完全消失
        if (moveTimer_ >= 0.5f) {
            isFullyCollected_ = true;
        }
		  worldTransform_.UpdateMatrix();
        return;
    }
	// 可以加入旋转、浮动等效果
	worldTransform_.rotation_.z += 0.05f; // 让道具缓慢旋转

	worldTransform_.UpdateMatrix();
	//色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
}

void Item::Draw(KamataEngine::Camera& camera) {
	 if (!isFullyCollected_) {
		   KamataEngine::Vector4 currentColor = color_;
        currentColor.w = alpha_; // 设置alpha通道
        objectColor_.SetColor(currentColor);
        model_->Draw(worldTransform_, camera,&objectColor_);
    }
}

// 获取道具的世界坐标
KamataEngine::Vector3 Item::GetWorldPosition() { return {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]}; }

// 处理拾取逻辑
void Item::Collect() {
	if (!isCollected_) {
		isCollected_ = true;
		moveTimer_ = 0.0f;
	}
	
}
bool Item::IsFullyCollected() const {
    return isFullyCollected_;
}