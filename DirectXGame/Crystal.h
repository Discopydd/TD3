#pragma once
#include "input/Input.h"

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

public:
	void Initialize();

	void Update();

	void UpdateSelection(int maxOptions);

	void FirstSelect();

	void SecondSelect();

	FirstCrystal GetFirstCrystal() const { return firstCrystal; }

	SecondCrystal GetSecondCrystal() const { return secondCrystal; }

	int GetSelectNum() const { return selectNum; }

	bool IsFirstCrystalGet() const { return isFirstCrystalGet; }
	bool IsSecondCrystalGet() const { return isSecondCrystalGet; }
	bool IsUIOpen() const { return isUIOpen; }

	FirstCrystal HaveFirstCrystal() const { return firstCrystal; }
	SecondCrystal HaveSecondCrystal() const { return secondCrystal; }

	void UIOpen() { isUIOpen = true; } 

private:
	KamataEngine::Input* input_ = nullptr;

	FirstCrystal firstCrystal = FirstCrystal::None;
	SecondCrystal secondCrystal = SecondCrystal::None;

	int selectNum = 0;

	bool isFirstCrystalGet = false;
	bool isSecondCrystalGet = false;
	bool isUIOpen = false;

	
};