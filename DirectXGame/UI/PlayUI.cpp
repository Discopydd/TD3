#include "PlayUI.h"
#include "base/TextureManager.h"
#include "../CrysTal.h"

PlayUI::~PlayUI() {
	delete hpBar;
	delete damageBar;
	delete crystalGet;
	delete selectFrame;
	delete selectFireCrystal;
	delete selectIceCrystal;
	delete selectWindCrystal;
	delete selectSoilCrystal;
	delete UIBack;
	delete fireCrystal;
	delete iceCrystal;
	delete windCrystal;
	delete soilCrystal;
	delete crystalFrame;
	delete expBar;
	delete expBarFrame;
	delete colon;
	delete lv;
	delete digit1;
	delete digit2;
	delete digit3;
}

void PlayUI::Initialize(float hp, KamataEngine::Input* input, Crystal* crystal) {
	maxHP = hp;
	currentHP = hp;
	input_ = input;
	crystal_ = crystal;

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

	// HPバー
	hpBar = KamataEngine::Sprite::Create(hpHandle, {15.0f, 50.0f});
	damageBar = KamataEngine::Sprite::Create(damageHandle, {15.0f, 50.0f});

	// クリスタル取得
	crystalGet = KamataEngine::Sprite::Create(crysralGetHandle, {0.0f, 0.0f});
	selectFrame = KamataEngine::Sprite::Create(selectHandle, framePos[selectNum]);
	selectFireCrystal = KamataEngine::Sprite::Create(selectFireHandle, {0.0f, 0.0f});
	selectIceCrystal = KamataEngine::Sprite::Create(selectIceHandle, {0.0f, 0.0f});
	selectWindCrystal = KamataEngine::Sprite::Create(selectWindHandle, {0.0f, 0.0f});
	selectSoilCrystal = KamataEngine::Sprite::Create(selectSoilHandle, {0.0f, 0.0f});
	UIBack = KamataEngine::Sprite::Create(UIBackHandle, {0.0f, 0.0f});
	fireCrystal = KamataEngine::Sprite::Create(fireCrystalHandle, {0.0f, 0.0f});
	iceCrystal = KamataEngine::Sprite::Create(iceCrystalHandle, {0.0f, 0.0f});
	windCrystal = KamataEngine::Sprite::Create(windCrystalHandle, {0.0f, 0.0f});
	soilCrystal = KamataEngine::Sprite::Create(soilCrystalHandle, {0.0f, 0.0f});
	crystalFrame = KamataEngine::Sprite::Create(crystalFrameHandle, {17.0f, 82.0f});

	// 経験値取得
	expBar = KamataEngine::Sprite::Create(expHandle, {0.0f, 0.0f});
	expBarFrame = KamataEngine::Sprite::Create(expFrameHandle, {0.0f, 0.0f});

	// レベル
	colon = KamataEngine::Sprite::Create(colonHandle, {1155.0f, -5.0f});
	lv = KamataEngine::Sprite::Create(levelHandle, {1130.0f, -5.0f});
	digit1 = KamataEngine::Sprite::Create(numberHandles[1], {15.0f, 120.0f});
	digit2 = KamataEngine::Sprite::Create(numberHandles[0], {15.0f, 120.0f});
	digit3 = KamataEngine::Sprite::Create(numberHandles[0], {15.0f, 120.0f});

	expBar->SetSize({0.0f, 32.0f});
	damageBar->SetSize({300.0f, 30.0f});
	colon->SetSize({40.0f, 40.0f});
	lv->SetSize({40.0f, 40.0f});
	digit1->SetSize({30.0f, 30.0f});
	digit2->SetSize({30.0f, 30.0f});
	digit3->SetSize({30.0f, 30.0f});
	fireCrystal->SetSize({48.0f, 48.0f});
	iceCrystal->SetSize({48.0f, 48.0f});
	windCrystal->SetSize({48.0f, 48.0f});
	soilCrystal->SetSize({48.0f, 48.0f});
}

void PlayUI::Update(float gainedExp) {
	UpdateHpBar();

	UpdateGetCrystal();

	UpdateEXP(gainedExp);

	UpdateLevelDisplay();

}

void PlayUI::Draw() {
	// ダメージバー（背景）を描画
	damageBar->Draw();

	// HPバー（前面）を描画
	hpBar->Draw();

	expBarFrame->Draw();
	expBar->Draw();

	lv->Draw();
	colon->Draw();
	if (level < 10) {
		digit1->Draw();
	} else if (level < 100) {
		digit1->Draw();
		digit2->Draw();
	} else {
		digit1->Draw();
		digit2->Draw();
		digit3->Draw();
	}

	crystalFrame->Draw();
	if (crystal_->IsFirstCrystalGet()) {
		Crystal::FirstCrystal first = crystal_->HaveFirstCrystal();
		switch (first) {
		case Crystal::FirstCrystal::Fire:
			fireCrystal->SetPosition(firstCrystalPos);
			fireCrystal->Draw();
			break;
		case Crystal::FirstCrystal::Ice:
			iceCrystal->SetPosition(firstCrystalPos);
			iceCrystal->Draw();
			break;
		case Crystal::FirstCrystal::Wind:
			windCrystal->SetPosition(firstCrystalPos);
			windCrystal->Draw();
			break;
		case Crystal::FirstCrystal::Soil:
			soilCrystal->SetPosition(firstCrystalPos);
			soilCrystal->Draw();
			break;
		}
	}
	if (crystal_->IsSecondCrystalGet()) {
		Crystal::SecondCrystal second = crystal_->HaveSecondCrystal();
		switch (second) {
		case Crystal::SecondCrystal::Fire:
			fireCrystal->SetPosition(secondCrystalPos);
			fireCrystal->Draw();
			break;
		case Crystal::SecondCrystal::Ice:
			iceCrystal->SetPosition(secondCrystalPos);
			iceCrystal->Draw();
			break;
		case Crystal::SecondCrystal::Wind:
			windCrystal->SetPosition(secondCrystalPos);
			windCrystal->Draw();
			break;
		case Crystal::SecondCrystal::Soil:
			soilCrystal->SetPosition(secondCrystalPos);
			soilCrystal->Draw();
			break;
		}
	}

	if (crystal_->IsUIOpen()) {
		if (!crystal_->IsFirstCrystalGet()) {
			crystalGet->Draw();
			selectFrame->Draw();
		} else if (crystal_->IsFirstCrystalGet() && !crystal_->IsSecondCrystalGet()) {
			UIBack->Draw();
			Crystal::FirstCrystal first = crystal_->HaveFirstCrystal();
			switch (first) {
			case Crystal::FirstCrystal::Fire:
				selectIceCrystal->Draw();
				selectWindCrystal->Draw();
				selectSoilCrystal->Draw();
				break;
			case Crystal::FirstCrystal::Ice:
				selectFireCrystal->Draw();
				selectWindCrystal->Draw();
				selectSoilCrystal->Draw();
				break;
			case Crystal::FirstCrystal::Wind:
				selectFireCrystal->Draw();
				selectIceCrystal->Draw();
				selectSoilCrystal->Draw();
				break;
			case Crystal::FirstCrystal::Soil:
				selectFireCrystal->Draw();
				selectIceCrystal->Draw();
				selectWindCrystal->Draw();
				break;
			}
			selectFrame->Draw();
		}
	}

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
	hpBar->SetSize({300.0f * hpRatio, 30.0f}); // 200が最大幅
}

void PlayUI::UpdateGetCrystal() { 

    if (crystal_->IsUIOpen()) {
		if (!crystal_->IsFirstCrystalGet()) {
			// 範囲外アクセス防止チェック
			if (crystal_->GetSelectNum() >= 0 && crystal_->GetSelectNum() < 4) {
				selectFrame->SetPosition(framePos[crystal_->GetSelectNum()]);
			}
		} else if (crystal_->IsFirstCrystalGet() && !crystal_->IsSecondCrystalGet()) {
			// 範囲外アクセス防止チェック
			if (crystal_->GetSelectNum() >= 0 && crystal_->GetSelectNum() < 3) {
				selectFrame->SetPosition(framePos[crystal_->GetSelectNum()]);
			}

			Crystal::FirstCrystal first = crystal_->HaveFirstCrystal();
			switch (first) {
			case Crystal::FirstCrystal::Fire:
				selectIceCrystal->SetPosition(secondPos[0]);
				selectWindCrystal->SetPosition(secondPos[1]);
				selectSoilCrystal->SetPosition(secondPos[2]);
				break;
			case Crystal::FirstCrystal::Ice:
				selectFireCrystal->SetPosition(secondPos[0]);
				selectWindCrystal->SetPosition(secondPos[1]);
				selectSoilCrystal->SetPosition(secondPos[2]);
				break;
			case Crystal::FirstCrystal::Wind:
				selectFireCrystal->SetPosition(secondPos[0]);
				selectIceCrystal->SetPosition(secondPos[1]);
				selectSoilCrystal->SetPosition(secondPos[2]);
				break;
			case Crystal::FirstCrystal::Soil:
				selectFireCrystal->SetPosition(secondPos[0]);
				selectIceCrystal->SetPosition(secondPos[1]);
				selectWindCrystal->SetPosition(secondPos[2]);
				break;
			}
		}
	}
}

void PlayUI::UpdateEXP(float gainedExp) { 
	if (level >= 4) {
        return; // 达到2级后经验条不再增长
    }
	if (!OpenGetUI) {
		currentExp += gainedExp; 
	}


	// レベルアップ処理
	if (currentExp >= maxExp) {
		currentExp -= maxExp;
		LevelUp();
	}

	// 経験値割合を計算
	float expRatio = currentExp / maxExp;

	// バーの横幅を更新
	expBar->SetSize({1280.0f * expRatio, 32.0f});
}

void PlayUI::LevelUp() { 
	 if (level >= 4) {
        return; // 限制最高等级为 4
    }

    currentExp = 0;  // 直接清零经验
 	maxExp *= 2.0f;     // レベルアップごとに必要経験値を増やす
	level++;            // レベルを1上げる
	OpenGetUI = true;
	crystal_->SetIsOpenUI(true);

	selectNum = 0;  // 让选择栏回到第一个选项
	selectFrame->SetPosition(framePos[selectNum]);
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
	lv->SetPosition({baseX, baseY});
	colon->SetPosition({baseX + 30.0f, baseY});

	// 桁数によって数字の表示位置を変更
	if (level < 10) {
		digit1->SetPosition({baseX + 60.0f, baseY + 5.0f});
		digit1->SetTextureHandle(numberHandles[ones]); // 1桁目の数字更新
	} else if (level < 100) {
		digit1->SetPosition({baseX + 60.0f, baseY + 5.0f});
		digit2->SetPosition({baseX + 80.0f, baseY + 5.0f});
		digit1->SetTextureHandle(numberHandles[tens]); // 2桁目の数字更新
		digit2->SetTextureHandle(numberHandles[ones]); // 1桁目の数字更新         // 3桁目は表示しない
	} else {
		digit1->SetPosition({baseX + 60.0f, baseY + 5.0f});
		digit2->SetPosition({baseX + 80.0f, baseY + 5.0f});
		digit3->SetPosition({baseX + 100.0f, baseY + 5.0f});
		digit1->SetTextureHandle(numberHandles[hundreds]); // 3桁目の数字更新
		digit2->SetTextureHandle(numberHandles[tens]);     // 2桁目の数字更新
	}
}

