#pragma once
#include<math/Vector3.h>
#include<3d/Camera.h>
#include <3d/Model.h>
#include <3d/WorldTransform.h>
#include <cassert>
#include <base/TextureManager.h>
class OrbitBullet {
protected:
    float angle_ = 0.0f;  // 子弹的角度
    float radius_ = 3.0f; // 子弹围绕玩家的半径
    int bulletCount_ = 4;  // 轨道子弹数量
    KamataEngine::Vector3* playerPosition_; // 指向玩家位置的指针
    KamataEngine::Vector3 velocity_;
    KamataEngine::Model* model_;
    KamataEngine::WorldTransform worldTransform_;
    uint32_t textureHandle_ = 0;
    bool isDead_ = false;

public:
     virtual void Initialize(KamataEngine::Model* model, KamataEngine::Vector3* playerPos, float initialAngle, int totalBullets);
    virtual void Update();
    virtual void Draw(const KamataEngine::Camera& camera);
     virtual void OnCollision();
     KamataEngine::Vector3 GetWorldPosition();
    bool IsDead() const { return isDead_; }
     float GetAngle() const { return angle_; } // 获取当前角度
     void SetBulletCount(int count) { bulletCount_ = count; }
};
