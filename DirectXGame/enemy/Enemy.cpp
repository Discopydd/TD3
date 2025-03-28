#include "Enemy.h"
#include "../scene/GameScene.h"

Enemy::Enemy() {}

Enemy::~Enemy() {  }

void Enemy::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;	
	velocity_ = {0, 0, -0.1f};
	LeaveVelo_ = {-1.0f, 1.0f, 0.0f};
	

}

void Enemy::Update() {
	if (knockbackTime_ > 0.0f) {
		worldTransform_.translation_ = myMath::Add(worldTransform_.translation_, knockbackVelocity_);
		knockbackTime_ -= 1.0f / 60.0f;
	} else {
	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}

	}
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(KamataEngine::Camera& camera) {
	model_->Draw(worldTransform_, camera); 

}

// 接近フェーズ
void Enemy::Approach() {
	// 计算向玩家移动的方向
	KamataEngine::Vector3 playerPos = player_->GetWorldPosition();
	KamataEngine::Vector3 direction = myMath::Subtract(playerPos, worldTransform_.translation_);
	// 计算到屏幕中心的向量
	//KamataEngine::Vector3 direction = myMath::Subtract(gameScene_->screenCenter, worldTransform_.translation_)
	float distance = myMath::Length(direction);
	float stopRadius = 1.0f; 
	if (distance > stopRadius) {
	   direction = myMath::Normalize(direction); // 单位化

	   // 计算朝向角度
	   float angleZ = atan2(direction.y, direction.x);
	   worldTransform_.rotation_.z = angleZ;

	   // 设置移动速度
	   const float speed = 0.1f;
	   KamataEngine::Vector3 velocity = myMath::Multiply(speed, direction);

	   // 更新位置
	   worldTransform_.translation_ = myMath::Add(worldTransform_.translation_, velocity);

	// 如果接近中心，可以改变阶段或销毁敌人
	//if (myMath::Length(myMath::Subtract(worldTransform_.translation_, gameScene_->screenCenter)) < 0.5f) {
	//	isDead_ = true; // 或切换到新的阶段
	//}
	}
	worldTransform_.UpdateMatrix(); // 更新矩阵
}


// 離脱フェーズ
void Enemy::Leave() {
	/*velocity_ = {-1.0f, 1.0f, 0.0f};*/
	// 移動(ベクトルを加算)
	worldTransform_.translation_ = myMath::Add(worldTransform_.translation_, LeaveVelo_);
}





// 衝突時コールバック
void Enemy::OnCollision() {
	isDead_ = true;
	if (gameScene_) {
		gameScene_->DropItem(GetWorldPosition(), false); // 小型敌人
	}
}

// ワールド座標を取得
KamataEngine::Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos; // 确保返回值
}

void Enemy::TakeKnockback(const KamataEngine::Vector3& direction, float force) {
	knockbackVelocity_ = myMath::Multiply(force, myMath::Normalize(direction));
	knockbackTime_ = 0.3f;
}