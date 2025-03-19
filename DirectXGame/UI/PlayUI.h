#pragma once
#include "2d/Sprite.h"
#include "input/Input.h"

class PlayUI {
public:
	enum class Element {
		none = -1,
		fire,
		ice,
		wind,
		soil,
	};

public:
	~PlayUI();

	void Initialize(float hp, KamataEngine::Input* input);

	void Update();

	void Draw();

	void SetCurrentHP(float hp) { currentHP = hp; }

	float GetCurrentHP() const { return currentHP; }

	int GetSelectedWeapon() const { return selectNum; }

	Element GetSelect() const { return element; }

	bool IsUIOpen() const { return OpenGetUI; } 

	void UpdateEXP(float gainedExp);
private:

	void UpdateGetCrystal();



	void LevelUp();

	void UpdateLevelDisplay();

private:
	KamataEngine::Input* input_ = nullptr;

	KamataEngine::Sprite* hpBar = nullptr;
	KamataEngine::Sprite* damageBar = nullptr;
	KamataEngine::Sprite* crystalGet = nullptr;
	KamataEngine::Sprite* selectFrame = nullptr;
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
	uint32_t expHandle = 0;
	uint32_t expFrameHandle = 0;
	uint32_t colonHandle = 0;
	uint32_t levelHandle = 0;
	uint32_t numberHandles[10];

	float maxHP = 0;
	float currentHP = 0;

	float currentExp = 0;
	float maxExp = 100.0f;
	int level = 1;

	bool OpenGetUI = false;

	KamataEngine::Vector2 framePos[4]{
	    {377.0f, 105.f},
        {377.0f, 249.f},
        {377.0f, 393.f},
        {377.0f, 537.f}
    };

	 int selectNum = 0;

	 Element element = Element::none;
};
