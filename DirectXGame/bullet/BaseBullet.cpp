#include "BaseBullet.h"

void BaseBullet::Initialize(KamataEngine::Model* model, KamataEngine::Vector3* playerPos, const KamataEngine::Vector3& velocity) {
   assert(model);

	model_ = model;
    playerPosition_ = playerPos;
	textureHandle_ = KamataEngine::TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
    worldTransform_.translation_ = *playerPos;
	velocity_ = velocity;
}

void BaseBullet::Update()
{
 if (delayTimer_ > 0) {
        delayTimer_--;
        // **确保子弹位置在延迟结束前仍然基于最新的玩家位置**
        if (playerPosition_) {
            worldTransform_.translation_ = *playerPosition_;
        }
        return; 
    }
	worldTransform_.translation_ += velocity_;

	 if (deathTimer_ > 0) { 
        deathTimer_--; 
    }

    if (deathTimer_ <= 0) {
        isDead_ = true;
    }

    worldTransform_.UpdateMatrix(); 
}

void BaseBullet::Draw(const KamataEngine::Camera& camera) {
    model_->Draw(worldTransform_, camera, textureHandle_);
}

void BaseBullet::OnCollision() {
    isDead_ = true;
}
KamataEngine::Vector3 BaseBullet::GetWorldPosition()
{
   KamataEngine::Vector3 worldPos;
    worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のX平行移動成分
    worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のY平行移動成分
    worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のZ平行移動成分
    return worldPos;
}