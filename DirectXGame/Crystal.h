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

	void UpdateSelection(int maxOptions);

	void FirstSelect();

	void SecondSelect();

	FirstCrystal GetFirstCrystal() const { return firstCrystal; }

	SecondCrystal GetSecondCrystal() const { return secondCrystal; }

	int GetSelectNum() const { return selectNum; }

	bool IsFirstCrystalGet() const { return isFirstCrystalGet; }
	bool IsSecondCrystalGet() const { return isSecondCrystalGet; }
	bool IsUIOpen() const { return isUIOpen; }

private:
	KamataEngine::Input* input_ = nullptr;

	FirstCrystal firstCrystal = FirstCrystal::None;
	SecondCrystal secondCrystal = SecondCrystal::None;

	int selectNum = 0;

	bool isFirstCrystalGet = false;
	bool isSecondCrystalGet = false;
	bool isUIOpen = false;

	
};