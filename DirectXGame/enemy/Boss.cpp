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
     worldTransform_.scale_ = { 0.1f, 0.1f, 0.1f }; // 初始很小
    spawnTimer_ = 0.0f;
    isSpawning_ = true;
     spawnDuration_ = 2.0f;
    hp_ = 100; // 设置 Boss 初始生命值
   phase_ = Phase::Approach; // 初始阶段为接近
}

void Boss::Update() {
    if (isSpawning_) {
        spawnTimer_ += 1.0f / 60.0f;
        float scale = spawnTimer_ / spawnDuration_; // 线性增长
        worldTransform_.scale_ = { scale * 2, scale * 2, scale * 2 };
        
        if (spawnTimer_ >= spawnDuration_) {
            isSpawning_ = false;
            worldTransform_.scale_ = { 2, 2, 2 }; // 最终大小
        }
        worldTransform_.UpdateMatrix();
        return;
    }
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
void Boss::Approach() {
    // 计算向玩家移动的方向
    KamataEngine::Vector3 playerPos = player_->GetWorldPosition();
    KamataEngine::Vector3 direction = myMath::Subtract(playerPos, worldTransform_.translation_);
    
    float distance = myMath::Length(direction);
    float stopRadius = 1.0f; 
    if (distance > stopRadius) {
        direction = myMath::Normalize(direction); // 单位化

        // 计算朝向角度
        float angleZ = atan2(direction.y, direction.x);
        worldTransform_.rotation_.z = angleZ;

        // 设置移动速度 - Boss速度是普通敌人的2倍
        const float speed = 0.15f; // 普通敌人是0.1f
        KamataEngine::Vector3 velocity = myMath::Multiply(speed, direction);

        // 更新位置
        worldTransform_.translation_ = myMath::Add(worldTransform_.translation_, velocity);
    }
	// 跳跃演出
	jumpTimer_ += 1.0f / 60.0f;
	float jumpOffset = std::abs(std::sin(jumpTimer_ * jumpSpeed_)) * jumpAmplitude_;
	worldTransform_.translation_.z = baseZ_ + jumpOffset;

    worldTransform_.UpdateMatrix(); // 更新矩阵
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
    return hp_ <= 0|| isDead_;;
}