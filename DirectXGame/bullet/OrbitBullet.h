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
     KamataEngine::Vector3 targetPosition_; // 目标位置
    KamataEngine::Model* model_;
    KamataEngine::WorldTransform worldTransform_;
    uint32_t textureHandle_ = 0;
    bool isDead_ = false;
    bool canDisappear_ = true; // 默认子弹会消失
    bool isOrbiting_ = true; // 是否在轨道上旋转
    int lifespan_ = 60; // 让子弹在发射后存在 3 秒（60帧 * 3）

float orbitSpeedIncrement_ = 0.002f; // 旋转速度增加量
bool isAcceleratingOrbit_ = false;
float maxRadius_;
public:
     virtual void Initialize(KamataEngine::Model* model, KamataEngine::Vector3* playerPos, float initialAngle, int totalBullets);
    virtual void Update();
    virtual void Draw(const KamataEngine::Camera& camera);
     virtual void OnCollision();
     KamataEngine::Vector3 GetWorldPosition();
    bool IsDead() const { return isDead_; }

     float GetAngle() const { return angle_; } // 获取当前角度
     void SetBulletCount(int count) { bulletCount_ = count; }
      void SetCanDisappear(bool canDisappear) { canDisappear_ = canDisappear; }
      void SetVelocity(KamataEngine::Vector3 velocity) { velocity_ = velocity; }
void SetOrbiting(bool orbiting) { isOrbiting_ = orbiting; }
void SetAcceleratingOrbit(bool isAccelerating, float speedIncrement) { isAcceleratingOrbit_ = isAccelerating;orbitSpeedIncrement_ = speedIncrement; maxRadius_ = radius_ * 2.0f;}

};
