#pragma once
#include <string>
#include <chrono>
#include "2d/Sprite.h"

class Timer {
public:
	~Timer();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 制限時間を設定
	void SetTimeLemit(float seconds) {  timeLimit_ = seconds;
    remainingTime = seconds; }

	// フラグが有効になるまでの時間を設定
	void SetTriggerTime(float seconds) { 
		triggerTime = seconds;
		triggered = false;
	}

	// 2つ目のフラグ設定
	void SetEnemyPwerUpTime(float seconds) { 
		enemyPwerUpTime = seconds;
		enemyPwerUp = false;
	}

	// 制限時間を超えたか
	bool IsTimeUp() const { return remainingTime <= 0.0f; }

	// トリガーが発動したか
	bool IsTriggered() const { return triggered; }
	bool IsEnemyPwerUp() const { return enemyPwerUp; }

	// 残り時間を取得
	float GetRemainingTime() const { return remainingTime > 0.0f ? remainingTime : 0.0f; }
	std::string GetFormattedTime() const;
	float GetCurrentTime() const;
private:
	  // 总时间限制
    float timeLimit_ = 0.0f;
    // 当前经过的时间（秒单位）
	float currentTime_ = 0.0f;
	// 残り時間（秒単位）
	float remainingTime = 0.0f;
	// トリガー時間
	float triggerTime = 0.0f;
	bool triggered = false;

	float enemyPwerUpTime = 0.0f;
	bool enemyPwerUp = false;
	// 1フレームの時間
	float deltaTime = 1.0f / 60.0f;

	KamataEngine::Sprite* minute1 = nullptr;
	KamataEngine::Sprite* minute2 = nullptr;
    KamataEngine::Sprite* colon = nullptr;
	KamataEngine::Sprite* second1 = nullptr;
    KamataEngine::Sprite* second2 = nullptr;

	uint32_t numberTextures[10];
	uint32_t colonTexture = 0;
};