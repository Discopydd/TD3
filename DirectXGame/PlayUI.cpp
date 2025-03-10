#include "PlayUI.h"
#include "base/TextureManager.h"

PlayUI::~PlayUI() {
	delete hpBar;
	delete damageBar;
	delete crystalGet;
	delete selectFrame;
	delete expBar;
	delete expBarFrame;
	delete colon;
	delete lv;
	delete digit1;
	delete digit2;
	delete digit3;
}

void PlayUI::Initialize(float hp, KamataEngine::Input* input) {
	maxHP = hp;
	currentHP = hp;
	input_ = input;

	hpHandle = KamataEngine::TextureManager::Load("HPbar.png");
	damageHandle = KamataEngine::TextureManager::Load("Damagebar.png");
	crysralGetHandle = KamataEngine::TextureManager::Load("crystalGetUI.png");
	selectHandle = KamataEngine::TextureManager::Load("selectFrame.png");
	expHandle = KamataEngine::TextureManager::Load("expBar.png");
	expFrameHandle = KamataEngine::TextureManager::Load("expBarFrame.png");
	colonHandle = KamataEngine::TextureManager::Load("numbers/colon.png");
	levelHandle = KamataEngine::TextureManager::Load("numbers/level.png");
	// 数字テクスチャを読み込む
	for (int i = 0; i < 10; ++i) {
		std::string fileName = "numbers/" + std::to_string(i) + ".png";
		numberHandles[i] = KamataEngine::TextureManager::Load(fileName.c_str());
	}

	// HPバー
	hpBar = KamataEngine::Sprite::Create(hpHandle, {15.0f, 50.0f});
	damageBar = KamataEngine::Sprite::Create(damageHandle, {15.0f, 50.0f});

	// クリスタル取得
	crystalGet = KamataEngine::Sprite::Create(crysralGetHandle, {0.0f, 0.0f});
	selectFrame = KamataEngine::Sprite::Create(selectHandle, framePos[selectNum]);

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
}

void PlayUI::Update(float gainedExp) {
	// HPバー
	if (currentHP < 0.0f)
		currentHP = 0.0f; // 下限チェック
	if (currentHP > maxHP)
		currentHP = maxHP; // 上限チェック
	if (input_->TriggerKey(DIK_G)) {
		OpenGetUI = true;
	}

	UpdateGetCrystal();

	UpdateEXP(gainedExp);

	UpdateLevelDisplay();

}

void PlayUI::Draw() {
	// HP割合を計算（0.0f～1.0f）
	float hpRatio = currentHP / maxHP;

	// HPバーのサイズを変更（横幅をHPに応じてスケール）
	hpBar->SetSize({300.0f * hpRatio, 30.0f}); // 200が最大幅

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

	if (OpenGetUI) {
		crystalGet->Draw();
		selectFrame->Draw();
	}

}

void PlayUI::UpdateGetCrystal() { 
    if (OpenGetUI) {
		// キー入力処理
		if (input_->TriggerKey(DIK_S)) {
			selectNum = (selectNum + 1) % 4; // 0〜3を循環
		} else if (input_->TriggerKey(DIK_W)) {
			selectNum = (selectNum + 3) % 4; // 循環 (4 + (-1) % 4 の処理)
		}

		// 範囲外アクセス防止チェック
		if (selectNum >= 0 && selectNum < 4) {
			selectFrame->SetPosition(framePos[selectNum]);
		}

		if (input_->TriggerKey(DIK_SPACE)) {
			OpenGetUI = false;
		}
	}
}

void PlayUI::UpdateEXP(float gainedExp) { 
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
	maxExp *= 1.2f;     // レベルアップごとに必要経験値を増やす
	level++;            // レベルを1上げる
	OpenGetUI = true;
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

