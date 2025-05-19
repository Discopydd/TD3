#include "Score.h"
#include "base/TextureManager.h"
#define NOMINMAX
#include <algorithm>

Score::~Score() {
	delete oneDigitsSprite;
	delete tenDigitsSprite;
	delete hundredDigitsSprite;
	delete thousandDigitsSprite;
	delete resultSprite;
}

void Score::Initialize() {
	score_ = 0;

	// 数字テクスチャを読み込む
	for (int i = 0; i < 10; ++i) {
		std::string fileName = "numbers/" + std::to_string(i) + ".png";
		numberTextures[i] = KamataEngine::TextureManager::Load(fileName.c_str());
	}

	resultTexture = KamataEngine::TextureManager::Load("score.png");

	thousandDigitsSprite = KamataEngine::Sprite::Create(numberTextures[0], {595.0f, 370.0f});
	hundredDigitsSprite = KamataEngine::Sprite::Create(numberTextures[0], {625.0f, 370.0f});
	tenDigitsSprite = KamataEngine::Sprite::Create(numberTextures[0], {655.0f, 370.0f});
	oneDigitsSprite = KamataEngine::Sprite::Create(numberTextures[0], {685.0f, 370.0f});

	resultSprite = KamataEngine::Sprite::Create(resultTexture, {0.0f, 0.0f});
}

void Score::Updata() {
	if (!isScoreDraw_) {
		return;
	}

	int score = (std::min)(score_, maxScore_);

	int one = score % 10;
	int ten = (score / 10) % 10;
	int hundred = (score / 100) % 10;
	int thousand = (score / 1000) % 10;

	thousandDigitsSprite->SetTextureHandle(numberTextures[thousand]);
	hundredDigitsSprite->SetTextureHandle(numberTextures[hundred]);
	tenDigitsSprite->SetTextureHandle(numberTextures[ten]);
	oneDigitsSprite->SetTextureHandle(numberTextures[one]);

	if (score_ < 10) {
		oneDigitsSprite->SetPosition({645.0f, 370.0});
	} else if (score_ >= 10 && score_ <= 99) {
		tenDigitsSprite->SetPosition({625.0f, 370.0f});
		oneDigitsSprite->SetPosition({655.0f, 370.0f});
	} else if (score_ >= 100 && score_ <= 999) {
		hundredDigitsSprite->SetPosition({615.0f, 370.0f});
		tenDigitsSprite->SetPosition({645.0f, 370.0f});
		oneDigitsSprite->SetPosition({675.0f, 370.0f});
	} else if (score >= 1000) {
		thousandDigitsSprite->SetPosition({595.0f, 370.0f});
		hundredDigitsSprite->SetPosition({625.0f, 370.0f});
		tenDigitsSprite->SetPosition({655.0f, 370.0f});
		oneDigitsSprite->SetPosition({685.0f, 370.0f});
	}
	
}


void Score::Draw() {
	if (!isScoreDraw_) {
		return;
	}

	resultSprite->Draw();

	if (thousandDigitsSprite && score_ >= 1000) {
		thousandDigitsSprite->Draw();
	}
	if (hundredDigitsSprite && score_ >= 100) {
		hundredDigitsSprite->Draw();
	}
	if (tenDigitsSprite && score_ >= 10) {
		tenDigitsSprite->Draw();
	}
	if (oneDigitsSprite) {
		oneDigitsSprite->Draw();
	}
}
