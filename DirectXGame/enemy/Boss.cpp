#include "Boss.h"
#include "../scene/GameScene.h"
#include <iostream>

Boss::Boss() {}

Boss::~Boss() {
}

void Boss::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) {
    // 调用基类的初始化
    Enemy::Initialize(model, position);
    worldTransform_.Initialize();
    worldTransform_.translation_ = position;
    worldTransform_.scale_ = { 2,2,2 };
    hp_ = 100; // 设置 Boss 初始生命值
   phase_ = Phase::Approach; // 初始阶段为接近
}

void Boss::Update() {
     if (hp_ <= 0) {
        isDead_ = true;
        return;
    }
	 if (knockbackTime_ > 0.0f) {
		 worldTransform_.translation_ = myMath::Add(worldTransform_.translation_, knockbackVelocity_);
		 knockbackTime_ -= 1.0f / 60.0f;
	 } else {
		 // 自己定义 Boss 行为，比如绕圈移动、追玩家等等
		 Approach(); // Boss 自己的
	 }

	 worldTransform_.UpdateMatrix(); // 更新世界变换
	 //#ifdef _DEBUG
//    KamataEngine::Vector3 worldPosition = worldTransform_.translation_;
//    ImGui::Text("hp %d", hp_);
//    ImGui::DragFloat3("pos", &worldTransform_.translation_.x);
//
//    ImGui::Text("isDead_ %d", isDead_);
//#endif
}   

void Boss::Draw(KamataEngine::Camera& camera) {
    // 使用基类的绘制逻辑
    Enemy::Draw(camera);
}

void Boss::TakeDamage(float damage) {
     float actualDamage = damage * player_->GetAttackPowerMultiplier();
    hp_ -= actualDamage;

      // 添加 knockback 效果（朝玩家方向相反）
	if (player_) {
		KamataEngine::Vector3 knockDir = myMath::Subtract(worldTransform_.translation_, player_->GetWorldPosition());
		TakeKnockback(knockDir, 0.05f); // 力度为 3，可调整
	}

   if (hp_ <= 0 && !isDead_) {  // 避免重复调用
        isDead_ = true;
        if (gameScene_) {
            gameScene_->DropItem(GetWorldPosition(), true); // true 表示Boss掉落
        }
    }
}

bool Boss::IsDead() const {
    return hp_ <= 0;
}