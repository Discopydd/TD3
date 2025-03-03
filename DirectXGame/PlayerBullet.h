#pragma once
#include<math/Vector3.h>
#include<3d/Camera.h>
#include <3d/Model.h>
#include <3d/WorldTransform.h>
#include <cassert>
#include <base/TextureManager.h>

enum class BulletType {
    Normal,
    Scatter,
    Fast,
    RapidFire,
	ScatterFast,
    RapidScatter,
    SurroundShot
};
// 子弹行为接口
class BulletBehavior {
public:
    virtual ~BulletBehavior() = default;
    virtual void Apply(class PlayerBullet& bullet) = 0;
};
class PlayerBullet {
	private:
	KamataEngine::Model* model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_;
	uint32_t textureHandle_ = 0;
	KamataEngine::Vector3 velocity_;

	  static const int32_t kLifeTime = 60 * 2;
    // 死亡timer
    int32_t deathTimer_ = kLifeTime;
    // 死亡mark
    bool isDead_ = false;
	BulletType bulletType_ = BulletType::Normal;

	 bool hasFired_ = false;
    std::vector<std::unique_ptr<BulletBehavior>> behaviors_;
public:
	~PlayerBullet();

	void Initialize(KamataEngine::Model* model,const KamataEngine::Vector3& position,const KamataEngine::Vector3& velocity, BulletType type);


	void Update();

	void Draw(const KamataEngine::Camera& camera);

	 bool IsDead() const { return isDead_; }

	 void OnCollision();

	 KamataEngine::Vector3 GetWorldPosition();

     void AddBehavior(std::unique_ptr<BulletBehavior> behavior) {
         behaviors_.push_back(std::move(behavior));
     }

    void SplitInto(const std::vector<KamataEngine::Vector3>& velocities,std::list<PlayerBullet*>& bullets);

    bool HasFired() const { return hasFired_; }
	  // 获取子弹速度
    const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

    // 设置子弹速度
    void SetVelocity(const KamataEngine::Vector3& newVelocity) { velocity_ = newVelocity; }

	std::list<PlayerBullet*>* bulletList = nullptr;

    void SetBulletList(std::list<PlayerBullet*>& bullets) {
        bulletList = &bullets;
    }

    std::list<PlayerBullet*>* GetBulletList() {
        return bulletList;
    }
    BulletType GetBulletType() const { return bulletType_; }

     void SetPosition(const KamataEngine::Vector3& newPos) {
        worldTransform_.translation_ = newPos;
        worldTransform_.UpdateMatrix();
    }

        KamataEngine::Model* GetModel() const {
        return model_;
    }
};