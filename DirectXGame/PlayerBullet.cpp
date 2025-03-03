#include "PlayerBullet.h"
#include <imgui.h>

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(KamataEngine::Model* model,const KamataEngine::Vector3& position,const KamataEngine::Vector3& velocity, BulletType type)
{
	assert(model);

	model_ = model;

	textureHandle_ = KamataEngine::TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	bulletType_ = type;
}

void PlayerBullet::Update()
{ 
	if (!hasFired_) {
        for (auto& behavior : behaviors_) {
            behavior->Apply(*this);
        }
        hasFired_ = true;
    }
	worldTransform_.translation_ += velocity_;

	   if (--deathTimer_ <= 0) {
        isDead_ = true;
    }

    worldTransform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(const KamataEngine::Camera& camera)
{
	model_->Draw(worldTransform_, camera, textureHandle_);
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

KamataEngine::Vector3 PlayerBullet::GetWorldPosition()
{
	KamataEngine::Vector3 worldPos;
    worldPos.x = worldTransform_.matWorld_.m[3][0]; // ワールド行列のX平行移動成分
    worldPos.y = worldTransform_.matWorld_.m[3][1]; // ワールド行列のY平行移動成分
    worldPos.z = worldTransform_.matWorld_.m[3][2]; // ワールド行列のZ平行移動成分
    return worldPos;
}
// 子弹分裂（用于 Scatter）
void PlayerBullet::SplitInto(const std::vector<KamataEngine::Vector3>& velocities,std::list<PlayerBullet*>& bullets) {
    for (const auto& vel : velocities) {
        PlayerBullet* newBullet = new PlayerBullet();
        newBullet->Initialize(model_, worldTransform_.translation_, vel, bulletType_);
        bullets.push_back(newBullet);
    }
}
