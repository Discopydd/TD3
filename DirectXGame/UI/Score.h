#pragma once 
#include "2d/Sprite.h"
#include "3d/WorldTransform.h"

class Score {
public:

	~Score();

	void Initialize();

	void Updata();

	void Draw();

	void GetEnemyScore() { score_ += 12; }

	void GetBossSocre() { score_ += 18; }

	bool IsScoreDraw(bool flag) { return isScoreDraw_ = flag; }

private:
	KamataEngine::WorldTransform worldTransform_;

	int score_ = 900;
	int maxScore_ = 9999;

	KamataEngine::Sprite* oneDigitsSprite = nullptr;      // 1桁目
	KamataEngine::Sprite* tenDigitsSprite = nullptr;      // 10桁目
	KamataEngine::Sprite* hundredDigitsSprite = nullptr;  // 100桁目
	KamataEngine::Sprite* thousandDigitsSprite = nullptr; // 1000桁目
	KamataEngine::Sprite* resultSprite = nullptr;

	uint32_t numberTextures[10];
	uint32_t resultTexture = 0;

	bool isScoreDraw_ = false;
};
