#pragma once
#include <KamataEngine.h>
#include <sstream>
#include "../Timer.h"
#include "../PlayUI.h"

using namespace KamataEngine;
class Enemy;
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

	Camera camera_;
	Model* model_ = nullptr;

	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	Timer* timer_ = nullptr;
	bool timerStart = false;

	PlayUI* ui_ = nullptr;

	float HP = 100;
};
