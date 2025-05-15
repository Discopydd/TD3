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
      isOrbiting_ = true; // 初始状态：环绕
    worldTransform_.Initialize();
    worldTransform_.rotation_.z = 3.1415926f * 1.5f;
    objectColor_ = std::make_unique<KamataEngine::ObjectColor>();
    objectColor_->Initialize();
    objectColor_->SetColor({0.0f, 0.0f, 1.0f, 1.0f}); // 默认蓝色
}

void OrbitBullet::Update() {
    if (!playerPosition_) return;

 if (isOrbiting_) {
        // 旋转阶段
        float baseSpeed = 0.05f;
        float speedMultiplier = 4.0f / max(1.0f, static_cast<float>(bulletCount_));
        float rotationSpeed = baseSpeed * speedMultiplier;
        
        if (isAcceleratingOrbit_) {
            rotationSpeed += orbitSpeedIncrement_;
            if (radius_ < maxRadius_) {
                radius_ += 0.02f;  // 适当增加半径
            }
        }
         angle_ += rotationSpeed;
        // 计算位置
        worldTransform_.translation_.x = playerPosition_->x + cos(angle_) * radius_;
        worldTransform_.translation_.y = playerPosition_->y + sin(angle_) * radius_;
        worldTransform_.translation_.z = playerPosition_->z;
    } else {
        // 发射阶段：子弹继续沿原方向飞行
        worldTransform_.translation_ += velocity_;
    }
        if (!isOrbiting_) {
            lifespan_--; // 只有在发射后才减少存活时间
        }
        if (lifespan_ <= 0) {
            isDead_ = true; // 标记子弹为待销毁
    hasHit_ = true; // 确保不会重复检测
        }

    worldTransform_.UpdateMatrix();
}
void OrbitBullet::Draw(const KamataEngine::Camera& camera) {
    model_->Draw(worldTransform_, camera, textureHandle_,objectColor_.get());
}

void OrbitBullet::OnCollision() {
    if (canDisappear_) { // 只有当 canDisappear_ 为 true 时，子弹才会消失
        isDead_ = true; // 标记子弹为待销毁
    hasHit_ = true; // 确保不会重复检测
    }
}
KamataEngine::Vector3 OrbitBullet::GetWorldPosition()
{
   KamataEngine::Vector3 worldPos;
    worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のX平行移動成分
    worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のY平行移動成分
    worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のZ平行移動成分
    return worldPos;
}