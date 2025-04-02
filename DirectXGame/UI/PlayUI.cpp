#include "PlayUI.h"
#include "base/TextureManager.h"
#include "../CrysTal.h"
#include "../UI/Timer.h"
#include <math/MathUtility.h>

PlayUI::~PlayUI() {
	delete hpBarSprite;
	delete damageBarSprite;
	delete crystalGetSprite;
	delete selectFrameSprite;
	delete selectFireCrystalSprite;
	delete selectIceCrystalSprite;
	delete selectWindCrystalSprite;
	delete selectSoilCrystalSprite;
	delete UIBackSprite;
	delete fireCrystalSprite;
	delete iceCrystalSprite;
	delete windCrystalSprite;
	delete soilCrystalSprite;
	delete crystalFrameSprite;
	delete selectPowerSprite;
	delete selectHPSprite;
	delete selectDefenseSprite;
	delete selectSpeedSprite;
	delete expBarSprite;
	delete expBarFrameSprite;
	delete colonSprite;
	delete lvSprite;
	delete digit1Sprite;
	delete digit2Sprite;
	delete digit3Sprite;
	delete gameClearSprite;
	delete gameOverSprite;
	delete titleGuideSprite;
}

void PlayUI::Initialize(float hp, KamataEngine::Input* input, Crystal* crystal, Timer* timer) {
	maxHP = hp;
	currentHP = hp;
	input_ = input;
	crystal_ = crystal;
	timer_ = timer;

	alpha = 0;

	hpHandle = KamataEngine::TextureManager::Load("HPbar.png");
	damageHandle = KamataEngine::TextureManager::Load("Damagebar.png");
	crysralGetHandle = KamataEngine::TextureManager::Load("crystalUI/crystalGetUI.png");
	selectHandle = KamataEngine::TextureManager::Load("crystalUI/selectFrame.png");
	selectFireHandle = KamataEngine::TextureManager::Load("crystalUI/selectFire.png");
	selectIceHandle = KamataEngine::TextureManager::Load("crystalUI/selectIce.png");
	selectWindHandle = KamataEngine::TextureManager::Load("crystalUI/selectWind.png");
	selectSoilHandle = KamataEngine::TextureManager::Load("crystalUI/selectSoil.png");
	UIBackHandle = KamataEngine::TextureManager::Load("crystalUI/UIBack.png");
	fireCrystalHandle = KamataEngine::TextureManager::Load("crystalUI/fireCrystal.png");
	iceCrystalHandle = KamataEngine::TextureManager::Load("crystalUI/iceCrystal.png");
	windCrystalHandle = KamataEngine::TextureManager::Load("crystalUI/windCrystal.png");
	soilCrystalHandle = KamataEngine::TextureManager::Load("crystalUI/soilCrystal.png");
	crystalFrameHandle = KamataEngine::TextureManager::Load("crystalUI/crystalFrame.png");
	selectPowerHandle = KamataEngine::TextureManager::Load("crystalUI/selectPower.png");
	selectHPHandle = KamataEngine::TextureManager::Load("crystalUI/selectHP.png");
	selectDefenseHandle = KamataEngine::TextureManager::Load("crystalUI/selectDefense.png");
	selectSpeedHandle = KamataEngine::TextureManager::Load("crystalUI/selectSpeed.png");
	expHandle = KamataEngine::TextureManager::Load("expBar.png");
	expFrameHandle = KamataEngine::TextureManager::Load("expBarFrame.png");
	colonHandle = KamataEngine::TextureManager::Load("numbers/colon.png");
	levelHandle = KamataEngine::TextureManager::Load("numbers/level.png");
	// 数字テクスチャを読み込む
	for (int i = 0; i < 10; ++i) {
		std::string fileName = "numbers/" + std::to_string(i) + ".png";
		numberHandles[i] = KamataEngine::TextureManager::Load(fileName.c_str());
	}
	//maxExp = 1000.0f * (static_cast<float>(pow(1.2f, 9.0f)) - 1.0f) / (1.2f - 1.0f);
	gameClearTexture = KamataEngine::TextureManager::Load("gameClear.png");
	gameOverTexture = KamataEngine::TextureManager::Load("gameOver.png");
	titleGuideTexture = KamataEngine::TextureManager::Load("titleGuide.png");

	// HPバー
	hpBarSprite = KamataEngine::Sprite::Create(hpHandle, {15.0f, 50.0f});
	damageBarSprite = KamataEngine::Sprite::Create(damageHandle, {15.0f, 50.0f});

	// クリスタル取得
	crystalGetSprite = KamataEngine::Sprite::Create(crysralGetHandle, {0.0f, 0.0f});
	selectFrameSprite = KamataEngine::Sprite::Create(selectHandle, framePos[selectNum]);
	selectFireCrystalSprite = KamataEngine::Sprite::Create(selectFireHandle, {0.0f, 0.0f});
	selectIceCrystalSprite = KamataEngine::Sprite::Create(selectIceHandle, {0.0f, 0.0f});
	selectWindCrystalSprite = KamataEngine::Sprite::Create(selectWindHandle, {0.0f, 0.0f});
	selectSoilCrystalSprite = KamataEngine::Sprite::Create(selectSoilHandle, {0.0f, 0.0f});
	UIBackSprite = KamataEngine::Sprite::Create(UIBackHandle, {0.0f, 0.0f});
	fireCrystalSprite = KamataEngine::Sprite::Create(fireCrystalHandle, {0.0f, 0.0f});
	iceCrystalSprite = KamataEngine::Sprite::Create(iceCrystalHandle, {0.0f, 0.0f});
	windCrystalSprite = KamataEngine::Sprite::Create(windCrystalHandle, {0.0f, 0.0f});
	soilCrystalSprite = KamataEngine::Sprite::Create(soilCrystalHandle, {0.0f, 0.0f});
	crystalFrameSprite = KamataEngine::Sprite::Create(crystalFrameHandle, {17.0f, 82.0f});

	// ステータスアップ
	selectPowerSprite = KamataEngine::Sprite::Create(selectPowerHandle, {0.0f, 0.0f});
	selectHPSprite = KamataEngine::Sprite::Create(selectHPHandle, {0.0f, 0.0f});
	selectDefenseSprite = KamataEngine::Sprite::Create(selectDefenseHandle, {0.0f, 0.0f});
	selectSpeedSprite = KamataEngine::Sprite::Create(selectSpeedHandle, {0.0f, 0.0f});

	// 経験値取得
	expBarSprite = KamataEngine::Sprite::Create(expHandle, {0.0f, 0.0f});
	expBarFrameSprite = KamataEngine::Sprite::Create(expFrameHandle, {0.0f, 0.0f});

	// レベル
	colonSprite = KamataEngine::Sprite::Create(colonHandle, {1155.0f, -5.0f});
	lvSprite = KamataEngine::Sprite::Create(levelHandle, {1130.0f, -5.0f});
	digit1Sprite = KamataEngine::Sprite::Create(numberHandles[1], {15.0f, 120.0f});
	digit2Sprite = KamataEngine::Sprite::Create(numberHandles[0], {15.0f, 120.0f});
	digit3Sprite = KamataEngine::Sprite::Create(numberHandles[0], {15.0f, 120.0f});

	// ゲームクリアorゲームオーバー
	gameClearSprite = KamataEngine::Sprite::Create(gameClearTexture, {0.0f, 0.0f});
	gameOverSprite = KamataEngine::Sprite::Create(gameOverTexture, {0.0f, 0.0f});
	titleGuideSprite = KamataEngine::Sprite::Create(titleGuideTexture, {0.0f, 0.0f});

	expBarSprite->SetSize({0.0f, 32.0f});
	damageBarSprite->SetSize({300.0f, 30.0f});
	colonSprite->SetSize({40.0f, 40.0f});
	lvSprite->SetSize({40.0f, 40.0f});
	digit1Sprite->SetSize({30.0f, 30.0f});
	digit2Sprite->SetSize({30.0f, 30.0f});
	digit3Sprite->SetSize({30.0f, 30.0f});
	fireCrystalSprite->SetSize({48.0f, 48.0f});
	iceCrystalSprite->SetSize({48.0f, 48.0f});
	windCrystalSprite->SetSize({48.0f, 48.0f});
	soilCrystalSprite->SetSize({48.0f, 48.0f});

	gameClearSprite->SetColor({1.0f, 1.0f, 1.0f, alpha});
	gameOverSprite->SetColor({1.0f, 1.0f, 1.0f, alpha});


}

void PlayUI::Update(float gainedExp) {
	UpdateHpBar();

	UpdateGetCrystal();

	UpdateEXP(gainedExp);

	UpdateLevelDisplay();

	UpdateGameClearOrOver();

}

void PlayUI::Draw() {
	// ダメージバー（背景）を描画
	damageBarSprite->Draw();

	// HPバー（前面）を描画
	hpBarSprite->Draw();

	// 経験値バー
	expBarFrameSprite->Draw();
	expBarSprite->Draw();

	// レベル
	lvSprite->Draw();
	colonSprite->Draw();
	if (level < 10) {
		digit1Sprite->Draw();
	} else if (level < 100) {
		digit1Sprite->Draw();
		digit2Sprite->Draw();
	} else {
		digit1Sprite->Draw();
		digit2Sprite->Draw();
		digit3Sprite->Draw();
	}

	// 取得クリスタル
	crystalFrameSprite->Draw();
	if (crystal_->IsFirstCrystalGet()) {
		Crystal::FirstCrystal first = crystal_->HaveFirstCrystal();
		switch (first) {
		case Crystal::FirstCrystal::Fire:
			fireCrystalSprite->SetPosition(firstCrystalPos);
			fireCrystalSprite->Draw();
			break;
		case Crystal::FirstCrystal::Ice:
			iceCrystalSprite->SetPosition(firstCrystalPos);
			iceCrystalSprite->Draw();
			break;
		case Crystal::FirstCrystal::Wind:
			windCrystalSprite->SetPosition(firstCrystalPos);
			windCrystalSprite->Draw();
			break;
		case Crystal::FirstCrystal::Soil:
			soilCrystalSprite->SetPosition(firstCrystalPos);
			soilCrystalSprite->Draw();
			break;
		}
	}
	if (crystal_->IsSecondCrystalGet()) {
		Crystal::SecondCrystal second = crystal_->HaveSecondCrystal();
		switch (second) {
		case Crystal::SecondCrystal::Fire:
			fireCrystalSprite->SetPosition(secondCrystalPos);
			fireCrystalSprite->Draw();
			break;
		case Crystal::SecondCrystal::Ice:
			iceCrystalSprite->SetPosition(secondCrystalPos);
			iceCrystalSprite->Draw();
			break;
		case Crystal::SecondCrystal::Wind:
			windCrystalSprite->SetPosition(secondCrystalPos);
			windCrystalSprite->Draw();
			break;
		case Crystal::SecondCrystal::Soil:
			soilCrystalSprite->SetPosition(secondCrystalPos);
			soilCrystalSprite->Draw();
			break;
		}
	}

	// クリスタル選択
	if (crystal_->IsUIOpen()) {
		if (!crystal_->IsFirstCrystalGet()) {
			crystalGetSprite->Draw();
			selectFrameSprite->Draw();
		} else if (crystal_->IsFirstCrystalGet() && !crystal_->IsSecondCrystalGet()) {
			UIBackSprite->Draw();
			Crystal::FirstCrystal first = crystal_->HaveFirstCrystal();
			switch (first) {
			case Crystal::FirstCrystal::Fire:
				selectIceCrystalSprite->Draw();
				selectWindCrystalSprite->Draw();
				selectSoilCrystalSprite->Draw();
				break;
			case Crystal::FirstCrystal::Ice:
				selectFireCrystalSprite->Draw();
				selectWindCrystalSprite->Draw();
				selectSoilCrystalSprite->Draw();
				break;
			case Crystal::FirstCrystal::Wind:
				selectFireCrystalSprite->Draw();
				selectIceCrystalSprite->Draw();
				selectSoilCrystalSprite->Draw();
				break;
			case Crystal::FirstCrystal::Soil:
				selectFireCrystalSprite->Draw();
				selectIceCrystalSprite->Draw();
				selectWindCrystalSprite->Draw();
				break;
			}
			selectFrameSprite->Draw();
		} else if (crystal_->IsFirstCrystalGet() && crystal_->IsSecondCrystalGet()) {
			UIBackSprite->Draw();

			// 2つの選択肢を取得
			const Crystal::StatusUP* statusOptions = crystal_->GetStatusOptions();

			// 1つ目の選択肢の描画
			switch (statusOptions[0]) {
			case Crystal::StatusUP::Power:
				selectPowerSprite->Draw();
				break;
			case Crystal::StatusUP::Hp:
				selectHPSprite->Draw();
				break;
			case Crystal::StatusUP::Defense:
				selectDefenseSprite->Draw();
				break;
			case Crystal::StatusUP::Speed:
				selectSpeedSprite->Draw();
				break;
			}

			// 2つ目の選択肢の描画
			switch (statusOptions[1]) {
			case Crystal::StatusUP::Power:
				selectPowerSprite->Draw();
				break;
			case Crystal::StatusUP::Hp:
				selectHPSprite->Draw();
				break;
			case Crystal::StatusUP::Defense:
				selectDefenseSprite->Draw();
				break;
			case Crystal::StatusUP::Speed:
				selectSpeedSprite->Draw();
				break;
			}

			// 選択枠を描画
			selectFrameSprite->Draw();
		}

	}

	if (timer_->IsTimeUp()) {
		gameClearSprite->Draw();
	} else if (currentHP <= 0) {
		gameOverSprite->Draw();
	}
	if (alpha >= 1) {
		titleGuideSprite->Draw();
	}
}

void PlayUI::SetMaxHP(float newMaxHP) { 
    maxHP = newMaxHP;  // 更新类成员变量
    // 确保当前 HP 不超过新的最大 HP
    if (currentHP > maxHP) {
        currentHP = maxHP;
    }
    UpdateHpBar();  // 更新 HP 条显示
}

void PlayUI::UpdateHpBar() {
	// HPバー
	if (currentHP < 0.0f)
		currentHP = 0.0f; // 下限チェック
	if (currentHP > maxHP)
		currentHP = maxHP; // 上限チェック

	// HP割合を計算（0.0f～1.0f）
	float hpRatio = currentHP / maxHP;

	// HPバーのサイズを変更（横幅をHPに応じてスケール）
	hpBarSprite->SetSize({300.0f * hpRatio, 30.0f}); // 200が最大幅
}

void PlayUI::UpdateGetCrystal() { 

    if (crystal_->IsUIOpen()) {
		if (!crystal_->IsFirstCrystalGet()) {
			// 範囲外アクセス防止チェック
			if (crystal_->GetSelectNum() >= 0 && crystal_->GetSelectNum() < 4) {
				selectFrameSprite->SetPosition(framePos[crystal_->GetSelectNum()]);
			}
		} else if (crystal_->IsFirstCrystalGet() && !crystal_->IsSecondCrystalGet()) {
			// 範囲外アクセス防止チェック
			if (crystal_->GetSelectNum() >= 0 && crystal_->GetSelectNum() < 3) {
				selectFrameSprite->SetPosition(framePos[crystal_->GetSelectNum()]);
			}

			Crystal::FirstCrystal first = crystal_->HaveFirstCrystal();
			switch (first) {
			case Crystal::FirstCrystal::Fire:
				selectIceCrystalSprite->SetPosition(secondPos[0]);
				selectWindCrystalSprite->SetPosition(secondPos[1]);
				selectSoilCrystalSprite->SetPosition(secondPos[2]);
				break;
			case Crystal::FirstCrystal::Ice:
				selectFireCrystalSprite->SetPosition(secondPos[0]);
				selectWindCrystalSprite->SetPosition(secondPos[1]);
				selectSoilCrystalSprite->SetPosition(secondPos[2]);
				break;
			case Crystal::FirstCrystal::Wind:
				selectFireCrystalSprite->SetPosition(secondPos[0]);
				selectIceCrystalSprite->SetPosition(secondPos[1]);
				selectSoilCrystalSprite->SetPosition(secondPos[2]);
				break;
			case Crystal::FirstCrystal::Soil:
				selectFireCrystalSprite->SetPosition(secondPos[0]);
				selectIceCrystalSprite->SetPosition(secondPos[1]);
				selectWindCrystalSprite->SetPosition(secondPos[2]);
				break;
			}
		} else if (crystal_->IsFirstCrystalGet() && crystal_->IsSecondCrystalGet()) {
			// 範囲外アクセス防止チェック
			if (crystal_->GetSelectNum() >= 0 && crystal_->GetSelectNum() < 2) {
				selectFrameSprite->SetPosition(statusFramePos[crystal_->GetSelectNum()]);
			}

			// 2つの選択肢を取得
			const Crystal::StatusUP* statusOptions = crystal_->GetStatusOptions();

			// 1つ目の選択肢の描画
			switch (statusOptions[0]) {
			case Crystal::StatusUP::Power:
				selectPowerSprite->SetPosition(statusPos[0]);
				break;
			case Crystal::StatusUP::Hp:
				selectHPSprite->SetPosition(statusPos[0]);
				break;
			case Crystal::StatusUP::Defense:
				selectDefenseSprite->SetPosition(statusPos[0]);
				break;
			case Crystal::StatusUP::Speed:
				selectSpeedSprite->SetPosition(statusPos[0]);
				break;
			}

			// 2つ目の選択肢の描画
			switch (statusOptions[1]) {
			case Crystal::StatusUP::Power:
				selectPowerSprite->SetPosition(statusPos[1]);
				break;
			case Crystal::StatusUP::Hp:
				selectHPSprite->SetPosition(statusPos[1]);
				break;
			case Crystal::StatusUP::Defense:
				selectDefenseSprite->SetPosition(statusPos[1]);
				break;
			case Crystal::StatusUP::Speed:
				selectSpeedSprite->SetPosition(statusPos[1]);
				break;
			}

		}
	}
}

void PlayUI::UpdateEXP(float gainedExp) { 
	if (!crystal_->IsUIOpen()) {
		currentExp += gainedExp; 
	}

	 const float lerpSpeed = 0.05f; // 调整过渡速度（值越大越快）
    displayExp = KamataEngine::MathUtility::Lerp(displayExp, currentExp, lerpSpeed);

    // 如果差值很小，直接设为目标值以避免无限接近
    if (std::abs(displayExp - currentExp) < 1.0f) {
        displayExp = currentExp;
    }
	// レベルアップ処理
	if (currentExp >= maxExp) {
		  if (displayExp >= maxExp) {
            currentExp = 0;
            displayExp = 0;
            LevelUp();
        }
	}

	// 経験値割合を計算
	float expRatio = displayExp / maxExp;

	// バーの横幅を更新
	expBarSprite->SetSize({1280.0f * expRatio, 32.0f});
}

void PlayUI::LevelUp() { 

 	maxExp *= 2.0f;     // レベルアップごとに必要経験値を増やす
	level++;            // レベルを1上げる
	crystal_->SetIsOpenUI(true);

	selectNum = 0;  // 让选择栏回到第一个选项
	selectFrameSprite->SetPosition(framePos[selectNum]);
}

void PlayUI::UpdateLevelDisplay() {
	// レベルの各桁を取得
	int ones = level % 10;             // 1の位
	int tens = (level / 10) % 10;      // 10の位
	int hundreds = (level / 100) % 10; // 100の位

	// 基準位置
	float baseX = 1130.0f; // Lvの位置（左端）
	float baseY = -5.0f; // Y座標は固定

	// 「Lv:」のスプライト位置を調整
	lvSprite->SetPosition({baseX, baseY});
	colonSprite->SetPosition({baseX + 30.0f, baseY});

	// 桁数によって数字の表示位置を変更
	if (level < 10) {
		digit1Sprite->SetPosition({baseX + 60.0f, baseY + 5.0f});
		digit1Sprite->SetTextureHandle(numberHandles[ones]); // 1桁目の数字更新
	} else if (level < 100) {
		digit1Sprite->SetPosition({baseX + 60.0f, baseY + 5.0f});
		digit2Sprite->SetPosition({baseX + 80.0f, baseY + 5.0f});
		digit1Sprite->SetTextureHandle(numberHandles[tens]); // 2桁目の数字更新
		digit2Sprite->SetTextureHandle(numberHandles[ones]); // 1桁目の数字更新         // 3桁目は表示しない
	} else {
		digit1Sprite->SetPosition({baseX + 60.0f, baseY + 5.0f});
		digit2Sprite->SetPosition({baseX + 80.0f, baseY + 5.0f});
		digit3Sprite->SetPosition({baseX + 100.0f, baseY + 5.0f});
		digit1Sprite->SetTextureHandle(numberHandles[hundreds]); // 3桁目の数字更新
		digit2Sprite->SetTextureHandle(numberHandles[tens]);     // 2桁目の数字更新
	}
}

void PlayUI::UpdateGameClearOrOver() {
	if (timer_->IsTimeUp()) {
		// フェードイン（α値を増やす）
		alpha += 0.01f;
		if (alpha > 1.0f)
			alpha = 1.0f;

		// スプライトに適用
		gameClearSprite->SetColor({1.0f, 1.0f, 1.0f, alpha});

	} else if (currentHP <= 0) {
		// フェードイン（α値を増やす）
		alpha += 0.01f;
		if (alpha > 1.0f)
			alpha = 1.0f;

		// スプライトに適用
		gameOverSprite->SetColor({1.0f, 1.0f, 1.0f, alpha});
	}
}