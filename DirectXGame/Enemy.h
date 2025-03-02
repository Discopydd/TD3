#pragma once
#include "3d/Model.h"
#include "base/TextureManager.h"
#include "3d/WorldTransform.h"
#include "assert.h"
#include "math/Vector3.h"
#include "myMath.h"

#include <list>
/// <summary>
/// 敵
/// </summary>
/// 

class Player;
// GameSceneクラスの前方宣言
class GameScene;

//行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};



class Enemy {
public:

	Enemy();
	~Enemy();

	virtual void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position);

	virtual void Update();

	virtual void Draw(KamataEngine::Camera& camera);

	// 接近フェーズ
	void Approach();
	// 離脱フェーズ
	void Leave();

	

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	virtual bool IsDead() const { return isDead_; }

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	

protected:

	GameScene* gameScene_ = nullptr; // 指向 GameScene 的指针

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 移動
	KamataEngine::Vector3 velocity_ = {0, 0, -0.1f};
	// 離脱時速度
	KamataEngine::Vector3 LeaveVelo_ = {-1.0f, 1.0f, 0.0f};
	// フェーズ
	Phase phase_ = Phase::Approach;



	
	// デスフラグ
	bool isDead_ = false;
};