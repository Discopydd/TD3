#include "Crystal.h"

void Crystal::Initialize() { 
	input_ = KamataEngine::Input::GetInstance();

	isUIOpen = true;
}

void Crystal::Uodate() {}

void Crystal::UpdateSelection(int maxOptions) {
	if (input_->TriggerKey(DIK_S)) {
		selectNum = (selectNum + 1) % maxOptions; // 選択肢を循環
	} else if (input_->TriggerKey(DIK_W)) {
		selectNum = (selectNum - 1 + maxOptions) % maxOptions; // 負数防止
	}
}

void Crystal::FirstSelect() {
	if (isUIOpen) {
		UpdateSelection(4);

		if (input_->TriggerKey(DIK_SPACE)) {
			constexpr FirstCrystal firstCrystalTable[4] = {FirstCrystal::Fire, FirstCrystal::Ice, FirstCrystal::Wind, FirstCrystal::Soil};

			firstCrystal = firstCrystalTable[selectNum];
			isUIOpen = false;
			isFirstCrystalGet = true;
		}
	}
}

void Crystal::SecondSelect() {
	if (isUIOpen && isFirstCrystalGet) {
		UpdateSelection(3);

		if (input_->TriggerKey(DIK_SPACE)) {
			constexpr SecondCrystal selectionTable[4][3] = {
			    {SecondCrystal::Ice,  SecondCrystal::Wind, SecondCrystal::Soil}, // Fire
			    {SecondCrystal::Fire, SecondCrystal::Wind, SecondCrystal::Soil}, // Ice
			    {SecondCrystal::Fire, SecondCrystal::Ice,  SecondCrystal::Soil}, // Wind
			    {SecondCrystal::Fire, SecondCrystal::Ice,  SecondCrystal::Wind}  // Soil
			};

			secondCrystal = selectionTable[static_cast<int>(firstCrystal)][selectNum];
			isUIOpen = false;
			isSecondCrystalGet = true;
		}
	}
}
