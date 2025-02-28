#pragma once
#include <KamataEngine.h>
#include <sstream>


using namespace KamataEngine;
#include <vector>
#include"../Player.h"
#include"../MapChipField.h"
#include "../CameraController.h"
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