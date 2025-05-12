#pragma once
#include <KamataEngine.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <list>

using namespace KamataEngine;
#include <vector>
#include"../player/Player.h"
#include"../map/MapChipField.h"
#include "../camera/CameraController.h"
#include "../enemy/Enemy.h"
#include "../enemy/Boss.h"
#include "../enemy/Item.h"
#include "../UI/Timer.h"
#include "../UI/PlayUI.h"
#include "../enemy/DeathParticles.h"
#include "../Crystal.h"
#include "../Field.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
	enum class Phase {
		Play,
		GameCler,
		GameOver,
	};

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	 void UpdateEnemySpawn();

	void SpawnEnemyNearPlayer();

	const KamataEngine::Vector3 screenCenter = {0.0f, 0.0f, 0.0f}; // 屏幕中心坐标

	void CheckAllcollisiions();

	
	void DropItem(const KamataEngine::Vector3& position, bool isBoss);

	void CreateDeathParticles(const KamataEngine::Vector3& position); // 生成死亡粒子的方法
	bool IsPlayerDead() const;

	void ChangePhase();

	bool IsFinished() const { return isFinished; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	WinApp* win = nullptr;

	// 自分の変数
	Camera camera_;
	Model* model_ = nullptr;
	//DebugCamera
	bool isDebugCameraActrive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	// Player
	Player* player_ = nullptr;

	//enemy
	uint32_t EnemytextureHandle_ = 0;
	// 3Dモデル
	Model* enemymodel_ = nullptr;
	Model* bossmodel_ = nullptr;
	Model* itemmodel_ = nullptr;
	 Model* modelField_ = nullptr;
	// 敌人
	Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemys_;

	  // 敌人生成参数
    float spawnTimer_ = 0.0f;
    float bossSpawnTimer_ = 0.0f;
    float spawnInterval_ = 10.0f;
    int enemyCount_ = 5;
    float bossSpawnInterval_ = 30.0f;
    int maxBossCount_ = 2;
    
    // 重置函数
    void ResetEnemySpawnParameters();
	// 敵の速度
	KamataEngine::Vector3 Velocity_ = {0, 0, -0.1f};


	std::list<Item*> items_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	// 待機中フラグ
	bool waitFlag = false;
	// 待機タイマー
	int32_t waitTimer = 0;
	//Map
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	MapChipField* mapChipField_;
	void GenerateBlocks();
	bool firstUpdateDone = false;
	// CameraController
	CameraController* cameraController_ = nullptr; 

	//自キャラの半径
	float Playerradius_ = 0.8f;
	//自弾の半径
	float PlayerBulletradius_ = 1.0f;
	//敵の半径
	float Enemyradius_ = 1.0f;
	//敵の弾の半径
	float EnemyBulletradius_ = 1.0f;

	Timer* timer_ = nullptr;
	//bool timerStart = false;

	Crystal* crystal_ = nullptr;

	PlayUI* ui_ = nullptr;

	float HP = 100;

	float exp = 0;

	bool isGamePaused = false; // 游戏是否暂停

	 std::list<DeathParticles*> deathParticlesList_;// 存储所有的死亡粒子

	 bool isFinished = false;
	 Phase phase = Phase::Play;

 	KamataEngine::Sprite* cursorSprite = nullptr;
	 uint32_t cursorTexture = 0;

	 KamataEngine::Vector2 pos = {0.0f, 0.0f};

	 float damageMultiplier_ = 1.0f; // 初始伤害倍率
	 float hpMultiplier_ = 1.0f;     // 敌人HP倍率

	 Field* field_ = nullptr;

	 uint32_t timeGetSEDataHandle_ = 0;
	 uint32_t timeGetSEVoiceHandle_ = 0;

   	uint32_t bgmDataHandle_ = 0;
	uint32_t bgmVoiceHandle_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};