#include "PlayUI.h"
#include "base/TextureManager.h"

PlayUI::~PlayUI() {
	delete hpBar;
	delete damageBar;
	delete crystalGet;
	delete selectFrame;
}

void PlayUI::Initialize(float hp, KamataEngine::Input* input) {
	maxHP = hp;
	currentHP = hp;
	input_ = input;

	hpHandle = KamataEngine::TextureManager::Load("HPbar.png");
	damageHandle = KamataEngine::TextureManager::Load("Damagebar.png");
	crysralGetHandle = KamataEngine::TextureManager::Load("crystalGetUI.png");
	selectHandle = KamataEngine::TextureManager::Load("selectFrame.png");

	hpBar = KamataEngine::Sprite::Create(hpHandle, {50.0f, 50.0f});
	damageBar = KamataEngine::Sprite::Create(damageHandle, {50.0f, 50.0f});
	crystalGet = KamataEngine::Sprite::Create(crysralGetHandle, {0.0f, 0.0f});
	selectFrame = KamataEngine::Sprite::Create(selectHandle, framePos[selectNum]);

	// ダメージバーは最大サイズ固定
	damageBar->SetSize({300.0f, 30.0f});
}

void PlayUI::Update() {
	// HPバー
	if (currentHP < 0.0f)
		currentHP = 0.0f; // 下限チェック
	if (currentHP > maxHP)
		currentHP = maxHP; // 上限チェック
	if (input_->TriggerKey(DIK_G)) {
		OpenGetUI = true;
	}

	UpdateGetCrystal();
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
	}
}

