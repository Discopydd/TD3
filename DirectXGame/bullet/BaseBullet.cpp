#include "BaseBullet.h"

void BaseBullet::Initialize(KamataEngine::Model* model, KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity,float rotationZ) {
   assert(model);

	model_ = model;
     initialPosition_ = position;
	textureHandle_ = KamataEngine::TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
    worldTransform_.translation_ = position;
     worldTransform_.rotation_.z = rotationZ; 
	velocity_ = velocity;
     delayTimer_ = 0;
    objectColor_ = std::make_unique<KamataEngine::ObjectColor>();
    objectColor_->Initialize();
    objectColor_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
}

void BaseBullet::Update()
{
 if (delayTimer_ > 0) {
        delayTimer_--;
        // **确保子弹位置在延迟结束前仍然基于最新的玩家位置**
  /*      if (playerPosition_) {
            worldTransform_.translation_ = *playerPosition_;
        }*/
         worldTransform_.UpdateMatrix();
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
    model_->Draw(worldTransform_, camera,textureHandle_,objectColor_.get());
}

void BaseBullet::OnCollision() {
    isDead_ = true; // 标记子弹为待销毁
    hasHit_ = true; // 确保不会重复检测
}
KamataEngine::Vector3 BaseBullet::GetWorldPosition()
{
   KamataEngine::Vector3 worldPos;
    worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のX平行移動成分
    worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のY平行移動成分
    worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のZ平行移動成分
    return worldPos;
}