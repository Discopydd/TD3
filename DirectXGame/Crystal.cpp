#include "Crystal.h"

void Crystal::Initialize() { 
	input_ = KamataEngine::Input::GetInstance();

	isFirstSelect = true;
	isUIOpne = true;
}

void Crystal::Uodate() {}

void Crystal::FirstSelect() {
	if (IsUIOpen && isFirstSelect) {
		// キー入力処理
		if (input_->TriggerKey(DIK_S)) {
			selectNum = (selectNum + 1) % 4; // 0〜3を循環
		} else if (input_->TriggerKey(DIK_W)) {
			selectNum = (selectNum + 3) % 4; // 循環 (4 + (-1) % 4 の処理)
		}

		if (input_->TriggerKey(DIK_SPACE)) {
			switch (selectNum) {
			case 0:
				firstCrystal = FirstCrystal::Fire;
				break;
			case 1:
				firstCrystal = FirstCrystal::Ice;
				break;
			case 2:
				firstCrystal = FirstCrystal::Wind;
				break;
			case 3:
				firstCrystal = FirstCrystal::Soil;
				break;
			}
			isUIOpne = false;
		}
	}
}

void Crystal::SecondSelect() {
	if (isUIOpne && isFirstSelect && isSecondSelect) {
		if (input_->TriggerKey(DIK_SPACE)) {
			switch (firstCrystal) {
			case Crystal::FirstCrystal::Fire:
				switch (selectNum) {
				case 0:
					secondCrystal = SecondCrystal::Ice;
					break;
				case 1:
					secondCrystal = SecondCrystal::Wind;
					break;
				case 2:
					secondCrystal = SecondCrystal::Soil;
					break;
				}
				break;
			case Crystal::FirstCrystal::Ice:
				switch (selectNum) {
				case 0:
					secondCrystal = SecondCrystal::Fire;
					break;
				case 1:
					secondCrystal = SecondCrystal::Wind;
					break;
				case 2:
					secondCrystal = SecondCrystal::Soil;
					break;
				}
				break;
			case Crystal::FirstCrystal::Wind:
				switch (selectNum) {
				case 0:
					secondCrystal = SecondCrystal::Fire;
					break;
				case 1:
					secondCrystal = SecondCrystal::Ice;
					break;
				case 2:
					secondCrystal = SecondCrystal::Soil;
					break;
				}
				break;
			case Crystal::FirstCrystal::Soil:
				switch (selectNum) {
				case 0:
					secondCrystal = SecondCrystal::Fire;
					break;
				case 1:
					secondCrystal = SecondCrystal::Ice;
					break;
				case 2:
					secondCrystal = SecondCrystal::Wind;
					break;
				}
				break;
			}
			isUIOpne = false;
		}
	}
}
