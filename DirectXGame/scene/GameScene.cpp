#include "GameScene.h"
#include <cassert>
#include <cmath> 
#include <fstream>
#include <cstdlib> // 用于随机数
#include <ctime>   // 用于获取时间

void GameScene::GenerateBlocks() {
	// ブロックを初期化
	const uint32_t kNumBlockHorizontal = MapChipField::kNumBlockHorizontal;
	const uint32_t kNumBlockVertical = MapChipField::kNumBlockVirtical;
	worldTransformBlocks_.resize(kNumBlockVertical);
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	// ブロック生成
for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			switch (mapChipField_->GetMapChipTypeByIndex(j, i)) {
			case MapChipType::kBlock:
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				break;
			}
		}
	}
}
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete enemymodel_;
	delete bossmodel_;
	delete debugCamera_;
	delete player_;
	delete cameraController_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
		delete mapChipField_;

		// 敵の解放
	    for (Enemy* enemy : enemys_) {
		    delete enemy;
	    }
	    enemys_.clear();
	 
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model_ = Model::Create();
	camera_.Initialize();
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);


	 //Map
	 mapChipField_ = new MapChipField;
	 mapChipField_->LoadMapChipCsv("Resources/map.csv");
	 GenerateBlocks();

	 //Player
	player_ = new Player();
	Vector3 playerPos = mapChipField_->GetMapChipPositionByIndex(3, 17);
	 player_->Initialize(&camera_,playerPos);
	 player_->SetMapChipField(mapChipField_);

	 //enemy
	 // 3Dモデルの生成
	 enemymodel_ = KamataEngine::Model::CreateFromOBJ("cube", true);
	 bossmodel_ = KamataEngine::Model::CreateFromOBJ("boss", true);


	  // CameraControll
	cameraController_ = new CameraController;
	cameraController_->Initialize(&camera_);
	CameraController::Rect cameraArea;

	cameraArea.left = 21.0f;
	cameraArea.right = 200.0f;
	cameraArea.bottom = -10.0f;
	cameraArea.top = 10.0f;

	cameraController_->SetMoveableArea(cameraArea);
	cameraController_->SetTarget(player_); // 追従したいターゲット
	cameraController_->Reset();               // 最初のカメラの位置を追従してるターゲットに設定していく
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActrive_ = !isDebugCameraActrive_;
	}
#endif // _DEBUG
	if (isDebugCameraActrive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}

	// Block
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->UpdateMatrix();
		}
	}
	
	player_->Update();
	cameraController_->Update();

	 UpdateEnemySpawn();

		// 敵の更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			model_->Draw(*worldTransformBlock, camera_);
		}
	}
	player_->Draw();

	for (Enemy* enemy : enemys_) {
		enemy->Draw(camera_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}



void GameScene::UpdateEnemySpawn() {
	static float spawnTimer = 0.0f;         // 生成普通敌人的计时器
	static float bossSpawnTimer = 0.0f;     // 生成 Boss 的计时器
	static float spawnInterval = 3.0f;      // 初始普通敌人生成间隔（秒）
	static int enemyCount = 1;              // 初始每次生成的普通敌人数量
	static float bossSpawnInterval = 10.0f; // Boss 生成的间隔（秒）
	static int maxBossCount = 2;            // 限制最多同时存在的 Boss 数量

	spawnTimer += 1.0f / 60.0f;     // 普通敌人计时
	bossSpawnTimer += 1.0f / 60.0f; // Boss 计时

	// 生成普通敌人
	if (spawnTimer >= spawnInterval) {
		spawnTimer = 0.0f;
		for (int i = 0; i < enemyCount; i++) {
			SpawnEnemyNearPlayer();
		}

		// 逐渐加快普通敌人生成速度 & 增加数量
		if (spawnInterval > 1.0f) {
			spawnInterval -= 0.1f;
		}
		enemyCount++;
	}

	// **生成 Boss 逻辑**
	int currentBossCount = 0;
	for (Enemy* enemy : enemys_) {
		if (dynamic_cast<Boss*>(enemy)) {
			currentBossCount++;
		}
	}

	// **满足条件才生成 Boss**
	if (bossSpawnTimer >= bossSpawnInterval && currentBossCount < maxBossCount) {
		bossSpawnTimer = 0.0f;   // 重置 Boss 计时器
		if (rand() % 100 < 20) { // 20% 概率生成 Boss（可以调整）
			
		}
	}
}



void GameScene::SpawnEnemyNearPlayer() {
	/*if (!player_)
		return;*/

	KamataEngine::Vector3 playerPos = player_->GetWorldPosition();
	float spawnDistance = 20.0f; // 生成的最小距离
	float maxDistance = 30.0f;   // 生成的最大距离

	float angle = (rand() % 360) * 3.14159265f / 180.0f; // 随机角度
	float distance = spawnDistance + (rand() % (int)(maxDistance - spawnDistance));

	float x = playerPos.x + cos(angle) * distance;
	float y = playerPos.y + sin(angle) * distance; // 保持 Y 轴高度不变
	float z = 0;

	KamataEngine::Vector3 spawnPosition = {x, y, z};

	   // **20% 概率生成 Boss**
	Enemy* newEnemy = nullptr;
	if (rand() % 100 < 20) { // 20% 概率
		newEnemy = new Boss();
		newEnemy->Initialize(bossmodel_, spawnPosition);
	} else {
		newEnemy = new Enemy();
		newEnemy->Initialize(enemymodel_, spawnPosition);
	}

	newEnemy->SetGameScene(this);
	newEnemy->SetPlayer(player_);
	enemys_.push_back(newEnemy);
}


