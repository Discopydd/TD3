#pragma once
#include<math/Vector3.h>
#include<3d/Camera.h>
#include <3d/Model.h>
#include <3d/WorldTransform.h>
#include <cassert>
#include <base/TextureManager.h>

class BaseBullet {
protected:
    KamataEngine::Model* model_;
    KamataEngine::WorldTransform worldTransform_;
    KamataEngine::Vector3 velocity_;
    uint32_t textureHandle_ = 0;
    KamataEngine::Vector3* playerPosition_ = nullptr;
    bool isDead_ = false;
    static const int32_t kLifeTime = 60 * 5;
    // 死亡timer
    int32_t deathTimer_ = kLifeTime;
    int delayTimer_ = 0; // 延迟计时器
     bool hasHit_ = false; // 标记子弹是否已击中敌人
public:
    virtual ~BaseBullet() {}
    virtual void Initialize(KamataEngine::Model* model, KamataEngine::Vector3* playerPos, const KamataEngine::Vector3& velocity);
    virtual void Update();
    virtual void Draw(const KamataEngine::Camera& camera);
    virtual void OnCollision();
    bool IsDead() const { return isDead_; }
     KamataEngine::Vector3 GetWorldPosition();
      void SetDelay(int delay) { delayTimer_ = delay; }
       bool HasHit() const { return hasHit_; }
    void SetHit(bool hit) { hasHit_ = hit; }
};