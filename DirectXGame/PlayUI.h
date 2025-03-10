#pragma once
#include "2d/Sprite.h"
#include "input/Input.h"

class PlayUI {
	enum class Element {
		fire,
		ice,
		wind,
		soil,
	};

public:
	~PlayUI();

	void Initialize(float hp, KamataEngine::Input* input);

	void Update(float gainedExp);

	void Draw();

	void SetCurrentHP(float hp) { currentHP = hp; }

	
private:

	void UpdateGetCrystal();

	void UpdateEXP(float gainedExp);

	void LevelUp();

private:
	KamataEngine::Input* input_ = nullptr;

	KamataEngine::Sprite* hpBar = nullptr;
	KamataEngine::Sprite* damageBar = nullptr;
	KamataEngine::Sprite* crystalGet = nullptr;
	KamataEngine::Sprite* selectFrame = nullptr;
	KamataEngine::Sprite* expBar = nullptr;
	KamataEngine::Sprite* expBarFrame = nullptr;

	uint32_t hpHandle = 0;
	uint32_t damageHandle = 0;
	uint32_t crysralGetHandle = 0;
	uint32_t selectHandle = 0;
	uint32_t expHandle = 0;
	uint32_t expFrameHandle = 0;

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

	int selectNum = 0;
};
