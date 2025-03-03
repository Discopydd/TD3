#include "Timer.h"
#include "base/TextureManager.h"

Timer::~Timer() { 
	delete minute1; 
	delete minute2;
	delete second1;
	delete second2;
	delete colon;
}

void Timer::Initialize() { 

	remainingTime = 0.0f; 
	triggered = false;

	// 数字テクスチャを読み込む
	for (int i = 0; i < 10; ++i) {
		std::string fileName = "numbers/" + std::to_string(i) + ".png";
		numberTextures[i] = KamataEngine::TextureManager::Load(fileName.c_str());
	}

	// コロンのテクスチャ
	colonTexture = KamataEngine::TextureManager::Load("numbers/colon.png");

	// 時間表示用のスプライトを作成
	minute1 = KamataEngine::Sprite::Create(numberTextures[0], {548.0f, 50.0f});
	minute2 = KamataEngine:: Sprite::Create(numberTextures[0], {570.0f, 50.0f});
	colon = KamataEngine::Sprite::Create(colonTexture, {590.0f, 50.0f});
	second1 = KamataEngine::Sprite::Create(numberTextures[0], {610.0f, 50.0f});
    second2 = KamataEngine::Sprite::Create(numberTextures[0], {634.0f, 50.0f});
}

void Timer::Update() {
	if (remainingTime > 0.0f) {
		remainingTime -= deltaTime;

	     // トリガー条件を満たしたらtrueにする
	    if (!triggered && remainingTime <= triggerTime) {
			triggered = true;
		}
		if (!enemyPwerUp && remainingTime <= enemyPwerUpTime) {
			enemyPwerUp = true;
		}
	}

}

void Timer::Draw() {
	int minutes = static_cast<int>(remainingTime) / 60;
	int seconds = static_cast<int>(remainingTime) % 60;

	// 各桁の数字を取得
	int min1 = minutes / 10; // 十の位
	int min2 = minutes % 10; // 一の位
	int sec1 = seconds / 10; // 十の位
	int sec2 = seconds % 10; // 一の位

	// テクスチャを切り替え
	minute1->SetTextureHandle(numberTextures[min1]);
	minute2->SetTextureHandle(numberTextures[min2]);
	second1->SetTextureHandle(numberTextures[sec1]);
	second2->SetTextureHandle(numberTextures[sec2]);

	// 描画
	if (minutes >= 10) {
    	minute1->Draw();
	}
	minute2->Draw();
	colon->Draw();
	second1->Draw();
	second2->Draw();
}

std::string Timer::GetFormattedTime() const { 
	    int minutes = static_cast<int>(remainingTime) / 60; // 分
	int seconds = static_cast<int>(remainingTime) % 60; // 秒

	// 分:秒の形式に整形 (例: "05:09")
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;

	return oss.str();
}
