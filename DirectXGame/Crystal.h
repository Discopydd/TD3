#pragma once
#include "input/Input.h"

class Crystal {
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

public:
	void Initialize();

	void Uodate();

	void FirstSelect();

	void SecondSelect();

	FirstCrystal GetFirstCrystal() const { return firstCrystal; }

	SecondCrystal GetSecondCrystal() const { return secondCrystal; }

	int GetSelectNum() const { return selectNum; }

	bool IsFirstSelect() const { return isFirstSelect; }
	bool IsSecondSelect() const { return isSecondSelect; }
	bool IsUIOpen() const { return isUIOpne; }

private:
	KamataEngine::Input* input_ = nullptr;

	FirstCrystal firstCrystal = FirstCrystal::None;
	SecondCrystal secondCrystal = SecondCrystal::None;

	int selectNum = 0;

	bool isFirstSelect = false;
	bool isSecondSelect = false;
	bool isUIOpne = false;

	
};