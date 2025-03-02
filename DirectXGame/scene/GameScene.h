#pragma once
#include <KamataEngine.h>
#include <sstream>


using namespace KamataEngine;
#include <vector>
#include"../Player.h"
#include"../MapChipField.h"
#include "../CameraController.h"
#include "../Enemy.h"
#include "../Boss.h"
#include <string>
#include <fstream>
#include <iostream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

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

		void EnemyPop(KamataEngine::Vector3 positon, const std::string& type);

	/// 敵発生データの読み込み
	void LoadEnemyPopData();

	// 敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	const KamataEngine::Vector3 screenCenter = {0.0f, 0.0f, 0.0f}; // 屏幕中心坐标

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

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
	KamataEngine::Model* enemymodel_ = nullptr;
	KamataEngine::Model* bossmodel_ = nullptr;
	// 敌人
	Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemys_;
	// 敵の座標
	KamataEngine::Vector3 Position = {2.0f, 0.0f, 60.0f};
	// 敵の速度
	KamataEngine::Vector3 Velocity_ = {0, 0, -0.1f};
	// 敵の離脱時速度
	KamataEngine::Vector3 LeaveVelo_ = {-1.0f, 1.0f, 0.0f};
	// 敵の半径
	float Enemyradius_ = 1.0f;

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
	// CameraController
	CameraController* cameraController_ = nullptr; 
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};