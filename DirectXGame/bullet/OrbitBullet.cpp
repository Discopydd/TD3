#include "OrbitBullet.h"
#include <cmath>
#include <algorithm>
void OrbitBullet::Initialize(KamataEngine::Model* model, KamataEngine::Vector3* playerPos, float initialAngle, int totalBullets) {
     assert(model);
    model_ = model;
    textureHandle_ = KamataEngine::TextureManager::Load("white1x1.png");
    playerPosition_ = playerPos;
    angle_ = initialAngle;
    bulletCount_ = totalBullets;

    worldTransform_.Initialize();
}

void OrbitBullet::Update() {
    if (!playerPosition_) return;

 // 计算旋转速度（子弹越少，速度越快）
    float baseSpeed = 0.05f;  // 基础旋转速度
    float speedMultiplier = 4.0f / max(1.0f, static_cast<float>(bulletCount_)); // 计算倍数
    float rotationSpeed = baseSpeed * speedMultiplier;  // 计算最终速度

    angle_ += rotationSpeed;  
    // 计算子弹位置
    worldTransform_.translation_.x = playerPosition_->x + cos(angle_) * radius_;
    worldTransform_.translation_.y = playerPosition_->y + sin(angle_) * radius_;
    worldTransform_.translation_.z = playerPosition_->z;

    worldTransform_.UpdateMatrix();
}
void OrbitBullet::Draw(const KamataEngine::Camera& camera) {
    model_->Draw(worldTransform_, camera, textureHandle_);
}

void OrbitBullet::OnCollision() {
    isDead_ = true;
}
KamataEngine::Vector3 OrbitBullet::GetWorldPosition()
{
   KamataEngine::Vector3 worldPos;
    worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のX平行移動成分
    worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のY平行移動成分
    worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のZ平行移動成分
    return worldPos;
}