#include "Timer.h"


void Timer::Initialize() { 
	remainingTime = 0.0f; 
	triggered = false;
}

void Timer::Update() {
	if (remainingTime > 0.0f) {
		remainingTime -= deltaTime;

		// トリガー条件を満たしたらtrueにする
		if (!triggered && remainingTime <= triggerTime) {
			triggered = true;
		}
	}
}

void Timer::Draw() {}

std::string Timer::GetFormattedTime() const { 
	    int minutes = static_cast<int>(remainingTime) / 60; // 分
	int seconds = static_cast<int>(remainingTime) % 60; // 秒

	// 分:秒の形式に整形 (例: "05:09")
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;

	return oss.str();
}
