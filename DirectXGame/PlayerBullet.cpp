#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(KamataEngine::Model* model,const KamataEngine::Vector3& position,const KamataEngine::Vector3& velocity)
{
	assert(model);

	model_ = model;

	textureHandle_ = KamataEngine::TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::Update()
{ 
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
