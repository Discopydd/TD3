#include "Crystal.h"
#include "2d/DebugText.h"

void Crystal::Initialize() { 
	input_ = KamataEngine::Input::GetInstance();
	isUIOpen = true;
}

void Crystal::Update() {
	if (!isUIOpen) {
		selectNum = 0;
	}
	KamataEngine::DebugText::GetInstance()->ConsolePrintf("First Crystal : %d\n", firstCrystal);
	KamataEngine::DebugText::GetInstance()->ConsolePrintf("Second Crystal : %d\n\n", secondCrystal);
	FirstSelect();
	SecondSelect();
}

void Crystal::UpdateSelection(int maxOptions) {
	if (input_->TriggerKey(DIK_S)) {
		selectNum = (selectNum + 1) % maxOptions; // 選択肢を循環
	} else if (input_->TriggerKey(DIK_W)) {
		selectNum = (selectNum - 1 + maxOptions) % maxOptions; // 負数防止
	}
}

void Crystal::FirstSelect() {
	if (isUIOpen && !isFirstCrystalGet) {
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
			// firstCrystal を除外した secondCrystal のリストを作成
			SecondCrystal selectedCrystals[3];
			int index = 0;

			// すべてのクリスタルを確認し、firstCrystal 以外を配列に格納
			for (int i = 1; i <= 4; i++) { // 1: Fire, 2: Ice, 3: Wind, 4: Soil
				if (static_cast<int>(firstCrystal) != i) {
					selectedCrystals[index++] = static_cast<SecondCrystal>(i);
				}
			}

			// 選択した secondCrystal を設定
			secondCrystal = selectedCrystals[selectNum];

			isUIOpen = false;
			isSecondCrystalGet = true;
		}
	}
}
