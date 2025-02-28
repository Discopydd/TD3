#pragma once
#include <string>
#include <chrono>

class Timer {
public:

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 制限時間を設定
	void SetTimeLemit(float seconds) { remainingTime = seconds; }

	// フラグが有効になるまでの時間を設定
	void SetTriggerTime(float seconds) { 
		triggerTime = seconds;
		triggered = false;
	}

	// 制限時間を超えたか
	bool IsTimeUp() const { return remainingTime <= 0.0f; }

	// トリガーが発動したか
	bool IsTriggered() const { return triggered; }

	// 残り時間を取得
	float GetRemainingTime() const { return remainingTime > 0.0f ? remainingTime : 0.0f; }
	std::string GetFormattedTime() const;

private:
	// 残り時間（秒単位）
	float remainingTime = 0.0f;
	// トリガー時間
	float triggerTime = 0.0f;
	bool triggered = false;
	// 1フレームの時間
	float deltaTime = 1.0f / 60.0f;
};