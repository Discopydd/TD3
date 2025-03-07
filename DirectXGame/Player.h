#pragma once
#include <3d/Model.h>
#include <input/input.h>
#include<3d/DebugCamera.h>
#include <3d/WorldTransform.h>
#include<3d/Camera.h>
#include"MapChipField.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include"AABB.h"
#include <numbers>

#define SCREEN_WIDTH 1280  // 你的游戏窗口宽度
#define SCREEN_HEIGHT 720  // 你的游戏窗口高度

#define PI 3.14159265358979323846f
using namespace KamataEngine;

class MapChipField;
class Player {
private:
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	Vector3 velocity_{};
	const float kAcceleration = 0.1f;
	const float kAttenuation = 0.2f;   // 速度減衰
	const float kLimitRunSpeed = 0.4f; // 最大速度

	float hp = 10;
	int damageCooldown = 0; 

	float turnStartRotationY_ = 0;
	float turnUseRotationY_ = 0; 
	int turnNowFram_ = 0;
	const int turnEndFrame_ = 10;

	const float kGravityAcceleration_ = 0.05f;

	//mapの判定変数
	MapChipField* mapChipField_ = nullptr;
	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;

	static inline const float kBlank = 0.01f;
	// マップと当たり情報
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move{};
	};
	//角
	enum Corner {
		kRightBottom,    //右下
		kLeftBottom,     //左下
		kRightTop,       //右上
		kLeftTop,        //左上

		kNumCorner      //要素数
	};

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	void MapCollision(CollisionMapInfo& info);

	void MapCollision_Up(CollisionMapInfo& info);

	void MapCollision_Down(CollisionMapInfo& info);

	void MapCollision_Left(CollisionMapInfo& info);

	void MapCollision_Right(CollisionMapInfo& info);



	public: 
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
	const WorldTransform& GetWorldTransform() { return worldTransform_; };

	const Vector3& GetVelocity() const { return velocity_; };

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

		// プレイヤーのHPを減らす関数
	void PlayerDamage();

	//AABBを取得
	AABB GetAABB();

	//衝突応答
	void OnCollision();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	
};