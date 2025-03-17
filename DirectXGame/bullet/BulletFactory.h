#pragma once
#include "BaseBullet.h"
#include "OrbitBullet.h"

enum class BulletType {
    Normal,   // 普通子弹
    Spread,   // 散射弹
    TripleShot,// 三连发
    Orbit,
    Accelerating // 新增加速子弹类型
};
class BulletFactory {
public:
    static std::vector<BaseBullet*> CreateBullet(BulletType type, KamataEngine::Model* model,
        KamataEngine::Vector3* position,
        const KamataEngine::Vector3& velocity,
        float rotation,KamataEngine::Vector3 acceleration = {0.0f, 0.0f, 0.0f}) {
        std::vector<BaseBullet*> bullets;

        switch (type) {
        case BulletType::Normal: {
            BaseBullet* bullet = new BaseBullet();  // 直接使用 BaseBullet
            bullet->Initialize(model, position, velocity);
            bullets.push_back(bullet);
            break;
        }
        case BulletType::Spread: {
            // 生成3颗散射弹
            for (int i = -1; i <= 1; ++i) {
                float angleOffset = i * 0.2f; // 每颗子弹的角度偏移
                KamataEngine::Vector3 spreadVelocity(
                    cos(rotation + angleOffset) * 1.0f,
                    sin(rotation + angleOffset) * 1.0f,
                    0
                );
                BaseBullet* bullet = new BaseBullet(); // 直接使用 BaseBullet
                bullet->Initialize(model, position, spreadVelocity);
                bullets.push_back(bullet);
            }
            break;
        }
        case BulletType::TripleShot: {
            for (int i = 0; i < 3; ++i) {
                KamataEngine::Vector3 tripleVelocity(
                    cos(rotation) * 1.0f,
                    sin(rotation) * 1.0f,
                    0
                );
                BaseBullet* bullet = new BaseBullet();
                bullet->Initialize(model, position, tripleVelocity);
                bullet->SetDelay(i * 5); // 让三连发的子弹间隔 5 帧发射
                bullets.push_back(bullet);
            }
            break;
        }
        case BulletType::Accelerating: {
           
            // 让子弹在 `Update` 内增加速度
            struct AcceleratingBullet : public BaseBullet {
                KamataEngine::Vector3 acceleration_;
                AcceleratingBullet(KamataEngine::Vector3 accel) : acceleration_(accel) {}

                void Update() override {
                    if (delayTimer_ > 0) {
                        delayTimer_--;
                        return;
                    }
                    velocity_ += acceleration_; // 速度增加
                    BaseBullet::Update();
                }
            };

            AcceleratingBullet* accelBullet = new AcceleratingBullet(acceleration);
            accelBullet->Initialize(model, position, velocity);
            bullets.push_back(accelBullet);
            break;
        }

        default: {
            BaseBullet* bullet = new BaseBullet();
            bullet->Initialize(model, position, velocity);
            bullets.push_back(bullet);
            break;
        }
        }
        return bullets;
    }
    static std::vector<OrbitBullet*> CreateBullet(BulletType type, KamataEngine::Model* model,
        KamataEngine::Vector3* playerPosition, int numBullets) {
        std::vector<OrbitBullet*> bullets;

        switch (type) {
        case BulletType::Orbit: {
            for (int i = 0; i < numBullets; ++i) {
                float angleOffset = (i / (float)numBullets) * (2.0f * 3.1415926f); // 均匀分布
                OrbitBullet* bullet = new OrbitBullet();
                bullet->Initialize(model, playerPosition, angleOffset, numBullets);
                bullets.push_back(bullet);
            }
            break;
        }
        default: break;
        }
        return bullets;
    }
};
