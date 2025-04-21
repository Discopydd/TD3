#include "Crystal.h"
#include "2d/DebugText.h"
#include <algorithm>
#include <random>

void Crystal::Initialize() { 
	input_ = KamataEngine::Input::GetInstance();
	audio_ = KamataEngine::Audio::GetInstance();
	isUIOpen = false;

	selectSEDataHandle_ = audio_->LoadWave("Audio/select.wav");
	decisionSEDataHandle_ = audio_->LoadWave("Audio/decision.wav");
}

void Crystal::Update() {
	if (!isUIOpen) {
		selectNum = 0;
	}
	KamataEngine::DebugText::GetInstance()->ConsolePrintf("StatusUP\nPower : %d\nHP = %d\nDefense = %d\nSpeed = %d\n\n", isPowerUp, isHpUp, isDefenseUp, isSpeedUp);
	FirstSelect();
	SecondSelect();
	ThirdSelect();
}

void Crystal::UpdateSelection(int maxOptions) {
	// 選択番号の変化チェック用
	int oldSelectNum = selectNum;

	// マウス位置取得
	KamataEngine::Vector2 mousePos = input_->GetMousePosition();

	// マウスによる選択処理
	for (int i = 0; i < maxOptions; ++i) {
		float left, right, top, bottom;

		if (isFirstCrystalGet && isSecondCrystalGet) {
			left = statusFramePos[i].x;
			right = statusFramePos[i].x + 500.0f;
			top = statusFramePos[i].y;
			bottom = statusFramePos[i].y + 100.0f;
		} else {
			left = framePos[i].x;
			right = framePos[i].x + 500.0f;
			top = framePos[i].y;
			bottom = framePos[i].y + 100.0f;
		}

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

	// 選択が変わったらSE再生
	if (oldSelectNum != selectNum) {
		selectSEVoiceHandle_ = audio_->PlayWave(selectSEDataHandle_, false, 0.5f);
	}
}


void Crystal::FirstSelect() {
	if (isUIOpen && !isFirstCrystalGet) {
		UpdateSelection(4);

		if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0)) {
			decisionSEVoiceHandle_ = audio_->PlayWave(decisionSEDataHandle_, false, 0.5f);
			constexpr FirstCrystal firstCrystalTable[4] = {FirstCrystal::Fire, FirstCrystal::Ice, FirstCrystal::Wind, FirstCrystal::Soil};

			firstCrystal = firstCrystalTable[selectNum];
			isUIOpen = false;
			isFirstCrystalGet = true;
		}
	}
}

void Crystal::SecondSelect() {
	if (isUIOpen && isFirstCrystalGet && !isSecondCrystalGet) {
		UpdateSelection(3);

		if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0)) {
			decisionSEVoiceHandle_ = audio_->PlayWave(decisionSEDataHandle_, false, 0.5f);
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

void Crystal::ThirdSelect() {
	if (isUIOpen && isFirstCrystalGet && isSecondCrystalGet) {
		// **すでに選択肢がセットされているか確認**
		if (!isThirdSelectionReady) {
			// 4つのステータスUPから2つをランダム選択
			std::array<StatusUP, 4> shuffledStatus = {StatusUP::Power, StatusUP::Hp, StatusUP::Defense, StatusUP::Speed};
			std::random_device rd;
			std::mt19937 g(rd());
			std::shuffle(shuffledStatus.begin(), shuffledStatus.end(), g);

			// 2つを選択肢として格納
			selectedStatus[0] = shuffledStatus[0];
			selectedStatus[1] = shuffledStatus[1];

			// **フラグをセットして、次のフレームで再シャッフルしないようにする**
			isThirdSelectionReady = true;
		}

		// 選択処理
		UpdateSelection(2);

		// 決定の処理
		if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0)) {
			decisionSEVoiceHandle_ = audio_->PlayWave(decisionSEDataHandle_, false, 0.5f);
			statusUp = selectedStatus[selectNum];

			ApplyStatusUp(statusUp);

			// **UIを閉じて、次のサイクルに進む**
			isUIOpen = false;
			isThirdSelectionReady = false; // 次回選択時に新しい選択肢を作る
		}
	}
}

void Crystal::ApplyStatusUp(StatusUP status) {
	switch (status) {
	case StatusUP::Power:
		isPowerUp = true;
		break;
	case StatusUP::Hp:
		isHpUp = true;
		break;
	case StatusUP::Defense:
		isDefenseUp = true;
		break;
	case StatusUP::Speed:
		isSpeedUp = true;
		break;
	}
}
