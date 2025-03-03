#pragma once
#include <3d/Model.h>
#include <input/input.h>
#include <3d/DebugCamera.h>
#include <3d/WorldTransform.h>
#include <3d/Camera.h>
#include "MapChipField.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include <numbers>
#include "AABB.h"
#include "PlayerBullet.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define PI 3.14159265358979323846f

using namespace KamataEngine;

class MapChipField;

class Player {
private:
    // 角色状态
    Camera* camera_ = nullptr;
    WorldTransform worldTransform_;
    Model* model_ = nullptr;
    Input* input_ = nullptr;

    Vector3 velocity_{};
    const float kAcceleration = 0.1f;
    const float kAttenuation = 0.2f;
    const float kLimitRunSpeed = 0.4f;

    // 碰撞检测
    MapChipField* mapChipField_ = nullptr;
    static inline const float kWidth = 2.0f;
    static inline const float kHeight = 2.0f;
    static inline const float kBlank = 0.01f;

    struct CollisionMapInfo {
        bool ceiling = false;
        bool landing = false;
        bool hitWall = false;
        Vector3 move{};
    };

    enum Corner {
        kRightBottom, kLeftBottom, kRightTop, kLeftTop, kNumCorner
    };

    Vector3 CornerPosition(const Vector3& center, Corner corner);
    void MapCollision(CollisionMapInfo& info);
    void MapCollision_Up(CollisionMapInfo& info);
    void MapCollision_Down(CollisionMapInfo& info);
    void MapCollision_Left(CollisionMapInfo& info);
    void MapCollision_Right(CollisionMapInfo& info);

    // 子弹管理
    std::list<PlayerBullet*> bullets_;
    BulletType currentBulletType_ = BulletType::Normal;

    int fireRate_ = 60;
    int fireTimer_ = 0;
    int rapidFireCount_ = 0;
    int rapidFireCooldown_ = 0;
    bool isRapidFiring_ = false;

public:
    ~Player();
    void Initialize(Camera* camera, const Vector3& position);
    void Update();
    void Draw();
    void Attack();

    const WorldTransform& GetWorldTransform() { return worldTransform_; }
    const Vector3& GetVelocity() const { return velocity_; }
    void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
    AABB GetAABB();
    void OnCollision();
    Vector3 GetWorldPosition();
    const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
    void SetBulletType(BulletType type) { currentBulletType_ = type; }
    void AddBullet(PlayerBullet* bullet) { bullets_.push_back(bullet); }
};
