#include "Boss.h"
#include <iostream>

Boss::Boss() {}

Boss::~Boss() {
	delete model_;
}

void Boss::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) {
    // 调用基类的初始化
    Enemy::Initialize(model, position);
    worldTransform_.Initialize();
    worldTransform_.translation_ = position;
    // 加载专属 Boss 的模型或贴图
    textureHandle_ = KamataEngine::TextureManager::Load("boss/boss.png");
    hp_ = 100; // 设置 Boss 初始生命值

	phase_ = Phase::Approach; // 初始阶段为接近
}

void Boss::Update() {
	Approach();

    worldTransform_.UpdateMatrix(); // 更新世界变换
#ifdef _DEBUG
    KamataEngine::Vector3 worldPosition = worldTransform_.translation_;
    ImGui::Text("hp %d", hp_);
    ImGui::DragFloat3("pos", &worldTransform_.translation_.x);

    ImGui::Text("isDead_ %d", isDead_);
#endif
}   

void Boss::Draw(KamataEngine::Camera& camera) {
    // 使用基类的绘制逻辑
    Enemy::Draw(camera);
}

void Boss::TakeDamage(int damage) {
    hp_ -= damage;
    if (hp_ <= 0) {
        isDead_ = true;
    }
}

bool Boss::IsDead() const {
    return hp_ <= 0;
}




