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
    Fast
};
class PlayerBullet {
	private:
	KamataEngine::Model* model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_;
	uint32_t textureHandle_ = 0;
	KamataEngine::Vector3 velocity_;

	  static const int32_t kLifeTime = 60 * 5;
    // 死亡timer
    int32_t deathTimer_ = kLifeTime;
    // 死亡mark
    bool isDead_ = false;
	BulletType bulletType_ = BulletType::Normal;
public:
	~PlayerBullet();

	void Initialize(KamataEngine::Model* model,const KamataEngine::Vector3& position,const KamataEngine::Vector3& velocity, BulletType type);


	void Update();

	void Draw(const KamataEngine::Camera& camera);

	 bool IsDead() const { return isDead_; }

	 void OnCollision();

	 KamataEngine::Vector3 GetWorldPosition();

};