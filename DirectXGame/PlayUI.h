#pragma once
#include "2d/Sprite.h"
#include "input/Input.h"

class Crystal;
class PlayUI {
	enum class Element {
		fire,
		ice,
		wind,
		soil,
	};

public:
	~PlayUI();

	void Initialize(float hp, KamataEngine::Input* input, Crystal* crystal);

	void Update(float gainedExp);

	void Draw();

	void SetCurrentHP(float hp) { currentHP = hp; }

	void UpdateHpBar();

	void UpdateGetCrystal();

	void UpdateEXP(float gainedExp);

	void LevelUp();

	void UpdateLevelDisplay();

private:
	KamataEngine::Input* input_ = nullptr;

	KamataEngine::Sprite* hpBar = nullptr;
	KamataEngine::Sprite* damageBar = nullptr;

	KamataEngine::Sprite* crystalGet = nullptr;
	KamataEngine::Sprite* selectFrame = nullptr;
	KamataEngine::Sprite* selectFireCrystal = nullptr;
	KamataEngine::Sprite* selectIceCrystal = nullptr;
	KamataEngine::Sprite* selectWindCrystal = nullptr;
	KamataEngine::Sprite* selectSoilCrystal = nullptr;
	KamataEngine::Sprite* UIBack = nullptr;
	KamataEngine::Sprite* fireCrystal = nullptr;
	KamataEngine::Sprite* iceCrystal = nullptr;
	KamataEngine::Sprite* windCrystal = nullptr;
	KamataEngine::Sprite* soilCrystal = nullptr;
	KamataEngine::Sprite* crystalFrame = nullptr;

	KamataEngine::Sprite* expBar = nullptr;
	KamataEngine::Sprite* expBarFrame = nullptr;

	KamataEngine::Sprite* colon = nullptr;
	KamataEngine::Sprite* lv = nullptr;
	KamataEngine::Sprite* digit1 = nullptr; // 1桁目
	KamataEngine::Sprite* digit2 = nullptr; // 2桁目
	KamataEngine::Sprite* digit3 = nullptr; // 3桁目

	uint32_t hpHandle = 0;
	uint32_t damageHandle = 0;

	uint32_t crysralGetHandle = 0;
	uint32_t selectHandle = 0;
	uint32_t selectFireHandle = 0;
	uint32_t selectIceHandle = 0;
	uint32_t selectWindHandle = 0;
	uint32_t selectSoilHandle = 0;
	uint32_t UIBackHandle = 0;
	uint32_t fireCrystalHandle = 0;
	uint32_t iceCrystalHandle = 0;
	uint32_t windCrystalHandle = 0;
	uint32_t soilCrystalHandle = 0;
	uint32_t crystalFrameHandle = 0;

	uint32_t expHandle = 0;
	uint32_t expFrameHandle = 0;

	uint32_t colonHandle = 0;
	uint32_t levelHandle = 0;
	uint32_t numberHandles[10];

	float maxHP = 0;
	float currentHP = 0;

	float currentExp = 0;
	float maxExp = 1000.0f;
	int level = 1;

	bool OpenGetUI = false;

	KamataEngine::Vector2 framePos[4]{
	    {377.0f, 105.f},
        {377.0f, 249.f},
        {377.0f, 393.f},
        {377.0f, 537.f}
    };
	KamataEngine::Vector2 secondPos[3] { 
		{384.0f, 112.f}, 
		{384.0f, 256.f}, 
		{384.0f, 400.f}
	};

	KamataEngine::Vector2 firstCrystalPos = {17.0f, 82.0f};
	KamataEngine::Vector2 secondCrystalPos = {80.0f, 82.0f};

	int selectNum = 0;

	Crystal* crystal_ = nullptr;
};
