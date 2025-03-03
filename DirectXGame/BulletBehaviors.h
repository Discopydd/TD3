#pragma once
#include "PlayerBullet.h"
#include <vector>
class FastBehavior : public BulletBehavior {
public:
    void Apply(PlayerBullet& bullet) override {
      KamataEngine::Vector3 velocity = bullet.GetVelocity();
      bullet.SetVelocity(velocity * 1.5f);
    }
};
class ScatterBehavior : public BulletBehavior {
public:

    void Apply(PlayerBullet& bullet) override {
    static constexpr float angleOffset = 0.15f;
    if (!bullet.HasFired()) { 
        std::vector<KamataEngine::Vector3> newVelocities;
        for (int i = -1; i <= 1; ++i) {
            float scatterAngle = atan2(bullet.GetVelocity().y, bullet.GetVelocity().x) + i * angleOffset;
            float velocityLength = Length(bullet.GetVelocity());
            newVelocities.push_back({
                cos(scatterAngle) * velocityLength,
                sin(scatterAngle) * velocityLength,
                0
            });
        }
        bullet.SplitInto(newVelocities,*bullet.GetBulletList());
    }
}
};
class ScatterFastBehavior : public BulletBehavior {
public:
    void Apply(PlayerBullet& bullet) override {
        KamataEngine::Vector3 velocity = bullet.GetVelocity();
        bullet.SetVelocity(velocity * 1.0f);
    }
};
class RapidFireBehavior : public BulletBehavior {
public:
    void Apply(PlayerBullet& bullet) override {
        bullet.SetVelocity(bullet.GetVelocity() * 1.2f);
    }
};

