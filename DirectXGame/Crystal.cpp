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
	// マウス位置取得
	KamataEngine::Vector2 mousePos = input_->GetMousePosition();

	// マウスによる選択処理
	for (int i = 0; i < maxOptions; ++i) {
		float left = framePos[i].x;
		float right = framePos[i].x + 500.0f;
		float top = framePos[i].y;
		float bottom = framePos[i].y + 100.0f;

		if (mousePos.x >= left && mousePos.x <= right && mousePos.y >= top && mousePos.y <= bottom) {
			selectNum = i;
			break;
		}
	}

	// キーボードによる選択処理
	if (input_->TriggerKey(DIK_S)) {
		selectNum = (selectNum + 1) % maxOptions;
	} else if (input_->TriggerKey(DIK_W)) {
		selectNum = (selectNum - 1 + maxOptions) % maxOptions;
	}
}

void Crystal::FirstSelect() {
	if (isUIOpen && !isFirstCrystalGet) {
		UpdateSelection(4);

		if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0)) {
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

		if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0)) {
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
