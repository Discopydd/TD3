#pragma once
#include "BaseBullet.h"
#include "OrbitBullet.h"

enum class BulletType {
    Normal,   // 普通子弹
    Spread,   // 散射弹
    TripleShot,// 三连发
    Orbit,
    Accelerating, // 新增加速子弹类型
    SpreadTripleShot,   // 散射弹三连发
    AcceleratingTripleShot,//加速三连发
    AcceleratingOrbit,//加快环绕速度，並扩大环绕范围
    AcceleratingSpread,//加速散单
    SpreadOrbit,//环绕子弹变为8个
    TripleShotOrbit,//环绕子弹不会消失

};
class BulletFactory {
public:



    static std::vector<BaseBullet*> CreateBullet(BulletType type, KamataEngine::Model* model,
        KamataEngine::Vector3* position,
        const KamataEngine::Vector3& velocity,
        float rotation, KamataEngine::Vector3 acceleration = { 0.0f, 0.0f, 0.0f }) {
        std::vector<BaseBullet*> bullets;

        
   
		
		static uint32_t wind = TextureManager::Load("bullet/wind.png");
		static uint32_t stone = TextureManager::Load("bullet/stone.png");

        switch (type) {
        case BulletType::Normal: {
            BaseBullet* bullet = new BaseBullet();  // 直接使用 BaseBullet
            bullet->Initialize(model, *position, velocity, rotation);
            bullet->objectColor_ = std::make_unique<ObjectColor>();
            bullet->objectColor_->Initialize();
            bullet->objectColor_->SetColor({0.0f, 1.0f, 0.5f, 1.0f});
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
                bullet->Initialize(model, *position, spreadVelocity, rotation);
                bullet->objectColor_ = std::make_unique<ObjectColor>();
                bullet->objectColor_->Initialize();
                bullet->objectColor_->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
				bullet->SetTexture(stone);
                bullets.push_back(bullet);
            }
            break;
        }
        case BulletType::TripleShot: {
            //for (int i = 0; i < 3; ++i) {
            //    KamataEngine::Vector3 tripleVelocity(
            //        cos(rotation) * 1.0f,
            //        sin(rotation) * 1.0f,
            //        0
            //    );
            //    BaseBullet* bullet = new BaseBullet();
            //    Vector3 spawnPos = *position;
            //    bullet->Initialize(model, spawnPos, tripleVelocity);
            //    bullet->SetDelay(i * 5); // 让三连发的子弹间隔 5 帧发射
            //    bullets.push_back(bullet);
            //}
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
            accelBullet->Initialize(model, *position, velocity, rotation);
            accelBullet->objectColor_ = std::make_unique<ObjectColor>();
            accelBullet->objectColor_->Initialize();
            accelBullet->objectColor_->SetColor({ 0.0f, 1.0f, 1.0f, 1.0f });
			accelBullet->SetTexture(wind);
            bullets.push_back(accelBullet);
            break;
        }
        case BulletType::SpreadTripleShot: {
            //for (int j = 0; j < 3; ++j) { // 三轮散射
            //    for (int i = -1; i <= 1; ++i) {
            //        float angleOffset = i * 0.2f; // 每颗子弹角度偏移
            //        KamataEngine::Vector3 spreadVelocity(
            //            cos(rotation + angleOffset) * 1.0f,
            //            sin(rotation + angleOffset) * 1.0f, 0);
            //        BaseBullet* bullet = new BaseBullet();
            //        bullet->Initialize(model, *position, spreadVelocity);
            //        bullet->SetDelay(j * 5); // 每轮子弹延迟 5 帧
            //        bullets.push_back(bullet);
            //    }
            //}
            break;
        }
        case BulletType::AcceleratingTripleShot: {
            // 生成三连发子弹，每颗子弹都有加速度
            struct AcceleratingBullet : public BaseBullet {
                KamataEngine::Vector3 acceleration_;
                AcceleratingBullet(KamataEngine::Vector3 accel) : acceleration_(accel) {}
                void Update() override {
                    velocity_ += acceleration_; // 每帧增加速度
                    BaseBullet::Update();
                }
            };
            //for (int j = 0; j < 3; ++j) { // 三连发
            //    KamataEngine::Vector3 accel(
            //        cos(rotation) * 0.02f,
            //        sin(rotation) * 0.02f, 0);
            //    AcceleratingBullet* bullet = new AcceleratingBullet(accel);
            //    bullet->Initialize(model, *position, velocity);
            //    bullet->SetDelay(j * 5); // 每发子弹间隔 5 帧
            //    bullets.push_back(bullet);
            //}
            break;
        }
        case BulletType::AcceleratingSpread: {
            struct AcceleratingBullet : public BaseBullet {
                KamataEngine::Vector3 acceleration_;
                AcceleratingBullet(KamataEngine::Vector3 accel) : acceleration_(accel) {}

                void Update() override {
                    velocity_ += acceleration_;  // 速度增加
                    BaseBullet::Update();
                }
            };

            for (int i = -1; i <= 1; ++i) {
                float angleOffset = i * 0.2f;
                KamataEngine::Vector3 spreadVelocity(
                    cos(rotation + angleOffset) * 1.0f,
                    sin(rotation + angleOffset) * 1.0f,
                    0
                );
                KamataEngine::Vector3 accel(
                    cos(rotation + angleOffset) * 0.02f,  // 轻微加速
                    sin(rotation + angleOffset) * 0.02f,
                    0
                );

                AcceleratingBullet* bullet = new AcceleratingBullet(accel);
                bullet->Initialize(model, *position, spreadVelocity, rotation);
                bullet->Initialize(model, *position, spreadVelocity, rotation);
                bullet->objectColor_ = std::make_unique<ObjectColor>();
                bullet->objectColor_->Initialize();
                bullet->objectColor_->SetColor({ 0.3f, 1.0f, 0.3f, 1.0f }); // 浅绿色
                bullets.push_back(bullet);
            }
            break;
        }
       default: break;
        }
        return bullets;
    }
    static std::vector<OrbitBullet*> CreateBullet(BulletType type, KamataEngine::Model* model,
        KamataEngine::Vector3* playerPosition, int numBullets) {
        std::vector<OrbitBullet*> bullets;

        static uint32_t ice = TextureManager::Load("bullet/ice.png");

        switch (type) {
        case BulletType::Orbit: {
            for (int i = 0; i < numBullets; ++i) {
                float angleOffset = (i / (float)numBullets) * (2.0f * 3.1415926f); // 均匀分布
                OrbitBullet* bullet = new OrbitBullet();
                bullet->Initialize(model, playerPosition, angleOffset, numBullets);
				bullet->SetTexture(ice);
                bullet->objectColor_ = std::make_unique<ObjectColor>();
                bullet->objectColor_->Initialize();
                bullet->objectColor_->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
                bullets.push_back(bullet);
            }
            break;
        }
        case BulletType::SpreadOrbit: {
            for (int i = 0; i < numBullets; ++i) {
                float angleOffset = (i / (float)numBullets) * (2.0f * 3.1415926f);
                OrbitBullet* bullet = new OrbitBullet();
                bullet->Initialize(model, playerPosition, angleOffset, numBullets);
                bullet->objectColor_ = std::make_unique<ObjectColor>();
                bullet->objectColor_->Initialize();
                bullet->objectColor_->SetColor({ 0.3f, 0.3f, 1.0f, 1.0f });
                bullets.push_back(bullet);
            }
            break;
        }
        case BulletType::TripleShotOrbit: {
            for (int i = 0; i < 4; ++i) {
                float angleOffset = (i / (float)numBullets) * (2.0f * 3.1415926f);
                OrbitBullet* bullet = new OrbitBullet();
                bullet->Initialize(model, playerPosition, angleOffset, numBullets);
                bullet->SetCanDisappear(false); // 你已经设置了这个
                bullet->SetAllowMultipleHit(true);
                bullet->objectColor_ = std::make_unique<ObjectColor>();
                bullet->objectColor_->Initialize();
                bullet->objectColor_->SetColor({ 0.7f, 0.3f, 1.0f, 1.0f });
                bullets.push_back(bullet);
            }
            break;
        }
        case BulletType::AcceleratingOrbit: {
            for (int i = 0; i < numBullets; ++i) {
                float angleOffset = (i / (float)numBullets) * (2.0f * 3.1415926f);
                OrbitBullet* bullet = new OrbitBullet();
                bullet->Initialize(model, playerPosition, angleOffset, numBullets);
                bullet->SetAcceleratingOrbit(true, 0.05f);  // 只对 AcceleratingOrbit 生效
                bullet->objectColor_ = std::make_unique<ObjectColor>();
                bullet->objectColor_->Initialize();
                bullet->objectColor_->SetColor({ 0.0f, 0.2f, 0.8f, 1.0f });
                bullets.push_back(bullet);
            }
            break;
        }
        default: break;
        }
        return bullets;
    }
};
