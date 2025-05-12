#pragma once
#include "input/Input.h"
#include "audio/audio.h"
#include "base/WinApp.h"

class Crystal {
public:
	enum class FirstCrystal {
		None,
		Fire,
		Ice,
		Wind,
		Soil,
	};
	enum class SecondCrystal {
		None,
		Fire,
		Ice,
		Wind,
		Soil,
	};

	enum class StatusUP {
		None,
		Power,
		Hp,
		Defense,
		Speed,
	};

public:
	void Initialize();

	void Update();

	void UpdateSelection(int maxOptions);

	void FirstSelect();

	void SecondSelect();

	void ThirdSelect();

	void ApplyStatusUp(StatusUP status);

	FirstCrystal HaveFirstCrystal() const { return firstCrystal; }
	SecondCrystal HaveSecondCrystal() const { return secondCrystal; }
	StatusUP SelectStatus() const { return statusUp; }

	void RestStatusSelect() { statusUp = StatusUP::None; }

	int GetSelectNum() const { return selectNum; }

	bool IsFirstCrystalGet() const { return isFirstCrystalGet; }
	bool IsSecondCrystalGet() const { return isSecondCrystalGet; }
	bool IsUIOpen() const { return isUIOpen; }
	bool IsPowerUP() const { return isPowerUp; }
	bool IsHpUP() const { return isHpUp; }
	bool IsSpeedUP() const { return isSpeedUp; }
	bool IsDefneseUP() const { return isDefenseUp; }
	const StatusUP* GetStatusOptions() const { return selectedStatus; }

	void SetIsOpenUI(bool flag) { isUIOpen = flag; }
	void SetIsPowerUP(bool flag) { isPowerUp = flag; }
	void SetIsUpUP(bool flag) { isHpUp = flag; }
	void SetIsSpeedUP(bool flag) { isSpeedUp = flag; }
	void SetIsDefenseUP(bool flag) { isDefenseUp = flag; }

	bool IsMouseInWindow(HWND hwnd);

private:
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Audio* audio_ = nullptr;
	KamataEngine::WinApp* win = nullptr;

	FirstCrystal firstCrystal = FirstCrystal::None;
	SecondCrystal secondCrystal = SecondCrystal::None;

	int selectNum = 0;

	bool isFirstCrystalGet = false;
	bool isSecondCrystalGet = false;
	bool isUIOpen = false;

	KamataEngine::Vector2 framePos[4]{
	    {377.0f, 105.f},
        {377.0f, 249.f},
        {377.0f, 393.f},
        {377.0f, 537.f}
    };
	KamataEngine::Vector2 statusFramePos[2]{
	    {377.0f, 239.0f},
	    {377.0f, 403.0f},
	};

	bool isPowerUp = false;
	bool isHpUp = false;
	bool isDefenseUp = false;
	bool isSpeedUp = false;

	StatusUP statusUp = StatusUP::None;
	StatusUP selectedStatus[2];

	bool isThirdSelectionReady = false;

	uint32_t selectSEDataHandle_ = 0;
	uint32_t decisionSEDataHandle_ = 0;

	uint32_t selectSEVoiceHandle_ = 0;
	uint32_t decisionSEVoiceHandle_ = 0;
};