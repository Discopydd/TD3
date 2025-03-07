#include "Boss.h"
#include <iostream>

Boss::Boss() {}

Boss::~Boss() {
}

void Boss::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) {
    // 调用基类的初始化
    Enemy::Initialize(model, position);
    worldTransform_.Initialize();
    worldTransform_.translation_ = position;
    hp_ = 100; // 设置 Boss 初始生命值
    isCircling_ = true; // 初始状态为绕场
}

void Boss::Update() {
    if (isCircling_) {
        CircleAround();
    }
    else {
        MoveToCenter();
    }

    worldTransform_.UpdateMatrix(); // 更新世界变换
#ifdef _DEBUG
    KamataEngine::Vector3 worldPosition = worldTransform_.translation_;
    ImGui::Text("hp %d", hp_);
    ImGui::DragFloat3("pos", &worldTransform_.translation_.x);
    ImGui::Text("isCircling_ %d", isCircling_);
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

void Boss::CircleAround() {
    const float radius = 18.0f; // 绕场的半径
    const float speed = 0.01f; // 绕场的速度

    // 更新角度
    angle_ += speed;
    if (angle_ >= 4 * M_PI) {
        angle_ = 0.0f;
        isCircling_ = false; // 完成绕场，切换为移动到中心
    }


    // 计算当前的目标位置（沿着圆轨迹）
    float newX = circleCenter_.x + radius * cos(angle_);
    float newY = circleCenter_.y + radius * sin(angle_);

    // 计算**当前前进方向**（上一帧位置 → 当前帧位置）
    KamataEngine::Vector3 prevPosition = worldTransform_.translation_;
    KamataEngine::Vector3 newPosition = { newX, newY, prevPosition.z }; // Z 轴保持不变

    KamataEngine::Vector3 direction = myMath::Subtract(newPosition, prevPosition);
    direction = myMath::Normalize(direction); // 归一化，确保方向向量长度为1

    // 计算绕Z轴旋转的角度，让Boss始终面向前进方向
    float angleZ = atan2(direction.y, direction.x);
    worldTransform_.rotation_.z = angleZ;

    // 更新位置
    worldTransform_.translation_ = newPosition;
    worldTransform_.UpdateMatrix();

    // 计算绕场的位置
    /*worldTransform_.translation_.x = circleCenter_.x + radius * cos(angle_);
    worldTransform_.translation_.y = circleCenter_.y + radius * sin(angle_);*/
}

void Boss::MoveToCenter() {
    // 移动到中心的逻辑
    KamataEngine::Vector3 direction = myMath::Subtract(circleCenter_, worldTransform_.translation_);
    direction = myMath::Normalize(direction);

    float angleZ = atan2(direction.y, direction.x);
    worldTransform_.rotation_.z = angleZ;

    const float speed = 0.1f; // 向中心移动的速度
    KamataEngine::Vector3 velocity = myMath::Multiply(speed, direction);

    // 更新位置
    worldTransform_.translation_ = myMath::Add(worldTransform_.translation_, velocity);

    // 如果接近中心，则标记死亡或其他逻辑
    if (myMath::Length(myMath::Subtract(worldTransform_.translation_, circleCenter_)) < 0.5f) {
        isDead_ = true;
    }
}
