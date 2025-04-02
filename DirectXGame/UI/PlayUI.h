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

	int GetSelectedWeapon() const { return selectNum; }

	int GetLevel() const { return level; }

private:
	void UpdateHpBar();

	void UpdateGetCrystal();

	void UpdateEXP(float gainedExp);

	void LevelUp();

	void UpdateLevelDisplay();

private:
	KamataEngine::Input* input_ = nullptr;

	KamataEngine::Sprite* hpBarSprite = nullptr;
	KamataEngine::Sprite* damageBarSprite = nullptr;

	KamataEngine::Sprite* crystalGetSprite = nullptr;
	KamataEngine::Sprite* selectFrameSprite = nullptr;
	KamataEngine::Sprite* selectFireCrystalSprite = nullptr;
	KamataEngine::Sprite* selectIceCrystalSprite = nullptr;
	KamataEngine::Sprite* selectWindCrystalSprite = nullptr;
	KamataEngine::Sprite* selectSoilCrystalSprite = nullptr;
	KamataEngine::Sprite* UIBackSprite = nullptr;
	KamataEngine::Sprite* fireCrystalSprite = nullptr;
	KamataEngine::Sprite* iceCrystalSprite = nullptr;
	KamataEngine::Sprite* windCrystalSprite = nullptr;
	KamataEngine::Sprite* soilCrystalSprite = nullptr;
	KamataEngine::Sprite* crystalFrameSprite = nullptr;

	KamataEngine::Sprite* selectPowerSprite = nullptr;
	KamataEngine::Sprite* selectHPSprite = nullptr;
	KamataEngine::Sprite* selectDefenseSprite = nullptr;
	KamataEngine::Sprite* selectSpeedSprite = nullptr;

	KamataEngine::Sprite* expBarSprite = nullptr;
	KamataEngine::Sprite* expBarFrameSprite = nullptr;

	KamataEngine::Sprite* colonSprite = nullptr;
	KamataEngine::Sprite* lvSprite = nullptr;
	KamataEngine::Sprite* digit1Sprite = nullptr; // 1桁目
	KamataEngine::Sprite* digit2Sprite = nullptr; // 2桁目
	KamataEngine::Sprite* digit3Sprite = nullptr; // 3桁目

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

	uint32_t selectPowerHandle = 0;
	uint32_t selectHPHandle = 0;
	uint32_t selectDefenseHandle = 0;
	uint32_t selectSpeedHandle = 0;

	uint32_t expHandle = 0;
	uint32_t expFrameHandle = 0;

	uint32_t colonHandle = 0;
	uint32_t levelHandle = 0;
	uint32_t numberHandles[10];

	float maxHP = 0;
	float currentHP = 0;

	float currentExp = 0;
	float displayExp = 0;
	float maxExp = 1000.0f;
	int level = 1;

	KamataEngine::Vector2 framePos[4]{
	    {377.0f, 105.f},
        {377.0f, 249.f},
        {377.0f, 393.f},
        {377.0f, 537.f}
    };

	KamataEngine::Vector2 secondPos[3]{
	    {384.0f, 112.f},
        {384.0f, 256.f},
        {384.0f, 400.f}
    };

	KamataEngine::Vector2 statusFramePos[2]{
	    {377.0f, 239.0f},
	    {377.0f, 403.0f},
	};
	KamataEngine::Vector2 statusPos[2]{
	    {384.0f, 246.0f},
	    {384.0f, 410.0f},
	};

	KamataEngine::Vector2 firstCrystalPos = {17.0f, 82.0f};
	KamataEngine::Vector2 secondCrystalPos = {80.0f, 82.0f};

	int selectNum = 0;

	Crystal* crystal_ = nullptr;

};
