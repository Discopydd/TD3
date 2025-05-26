#pragma once
#include <3d/Model.h>
#include <input/input.h>
#include<3d/DebugCamera.h>
#include <3d/WorldTransform.h>
#include<3d/Camera.h>
#include "audio/Audio.h"
#include"../map/MapChipField.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include"../AABB.h"
#include <numbers>
#include "../bullet/BulletFactory.h"
#include "../UI/PlayUI.h"
#include"../Crystal.h"
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
	Model* bulletModel_ = nullptr;
	Input* input_ = nullptr;

	Vector3 velocity_{};
	const float kAcceleration = 0.1f;
	const float kAttenuation = 0.2f;   // 速度減衰
	const float kLimitRunSpeed = 0.2f; // 最大速度


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

	void ShowImGuiControls();

	void MapCollision(CollisionMapInfo& info);

	void MapCollision_Up(CollisionMapInfo& info);

	void MapCollision_Down(CollisionMapInfo& info);

	void MapCollision_Left(CollisionMapInfo& info);

	void MapCollision_Right(CollisionMapInfo& info);

	bool IsOrbitBulletType(BulletType type);
	void CheckGroundCollision();
	 bool IsTripleShotType(BulletType type) const {
        return type == BulletType::TripleShot || type == BulletType::AcceleratingTripleShot||type == BulletType::SpreadTripleShot;
    }
	BulletType bulletType_ = BulletType::Normal;
	BulletType previousBulletType_ = BulletType::Normal; // 记录上一次的子弹类型
    std::list<BaseBullet*> bullets_;
	std::list<OrbitBullet*> orbitBullets_;  // 存储环绕子弹

	  int fireRate_ = 60;
    int fireTimer_ = 0;
	float bulletSpeed_ = 1.0f; // 子弹初始速度
    float acceleration_ = 0.04f; // 加速子弹的加速度
    int orbitBulletCount_ = 0;  // 轨道子弹数量

	PlayUI* ui_ = nullptr; // 记录 UI 对象
    float HP = 100.0f; // 玩家生命值
	 float invincibleTime = 0.0f; // 受伤后的无敌时间（秒）
    const float invincibleDuration = 0.5f; // 受伤后无敌0.5秒
	// 击退相关变量
    Vector3 knockbackVelocity_{0, 0, 0};  // 初始化为零向量
    float knockbackDecay_ = 0.9f;
    bool isKnockback_ = false;
    bool isGrounded_ = false;
    const float kGroundCheckDistance = 0.1f; // 地面检测距离
    const float kGravity = -0.02f;          // 重力加速度
    float verticalVelocity_ = 0.0f;         // 垂直速度(Z轴)
	  // 平滑伤害相关
    float pendingDamage_ = 0.0f;      // 待处理的伤害
    float damagePerFrame_ = 0.0f;     // 每帧扣除的伤害
    const float damageDuration_ = 1.0f; // 伤害分摊时间（秒）
    bool isTakingDamage_ = false;     // 是否正在处理伤害

	 float baseAttackPower_ = 10.0f;  // 基本攻撃力
    float attackPowerMultiplier_ = 1.0f;  // 攻撃力倍率
    float baseMaxHP_ = 100.0f;  // 基本最大HP
    float defenseMultiplier_ = 1.0f;  // 防御倍率 (1.0 = 100%ダメージ)
    float baseMoveSpeed_ = 0.4f;  // 基本移動速度
    float moveSpeedMultiplier_ = 1.0f;  // 移動速度倍率

	int tripleShotCounter_ = 0;  // 当前已发射的子弹数（0~2）
    int tripleShotTimer_ = 0;    // 子弹生成间隔计时器
    const int kTripleShotInterval = 5; // 每 5 帧生成一个子弹
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

	//AABBを取得
	AABB GetAABB();

	//衝突応答
	void OnCollision();

	//ワールド座標を取得
	Vector3 GetWorldPosition();


	void Attack();
		// 弾リストを取得
	const std::list<BaseBullet*>& GetBullets() const { return bullets_; }
	  // 返回环绕子弹列表
    const std::list<OrbitBullet*>& GetOrbitBullets() const { return orbitBullets_; }

	void SetBulletType(BulletType type) { bulletType_ = type; }

	void TakeDamage(float damage, const Vector3& attackerPosition);

	void SetUI(PlayUI* ui) { ui_ = ui; }
	float GetHP() const { return HP; }

	void ApplyStatusUp(Crystal::StatusUP status);

	    float GetAttackPower() const { return baseAttackPower_ * attackPowerMultiplier_; }
    float GetMaxHP() const { return baseMaxHP_; }
    float GetDefenseRate() const { return defenseMultiplier_; } // 防御率 (0.5 = 50%ダメージ)
    float GetMoveSpeed() const { return baseMoveSpeed_ * moveSpeedMultiplier_; }
	float GetAttackPowerMultiplier() const { return attackPowerMultiplier_; }

	uint32_t fire = 0;

	Audio* audio_ = nullptr;
	uint32_t damagedSEDataHandle_ = 0;
	uint32_t damagedSEVoiceHandle_ = 0;
};