#include "GameScene.h"
#include <cassert>
#include <cmath> 
#include <fstream>
#include <cstdlib> 

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
	delete debugCamera_;
	delete enemymodel_;
	delete bossmodel_;
	delete player_;
	delete cameraController_;
	delete timer_;
	delete ui_;
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
		   for (Item* item : items_) {
		    delete item;
	    }
	    items_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model_ = Model::Create();
	camera_.Initialize();
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	 timer_ = new Timer();
	 timer_->Initialize();
	 timer_->SetTimeLemit(180.0f);
	 timer_->SetTriggerTime(30.0f);
	 timer_->SetEnemyPwerUpTime(12.0f);

	 ui_ = new PlayUI();
	 ui_->Initialize(HP,input_);

	 //Map
	 mapChipField_ = new MapChipField;
	 mapChipField_->LoadMapChipCsv("Resources/map.csv");
	 GenerateBlocks();

	 //Player
	player_ = new Player();
	Vector3 playerPos = mapChipField_->GetMapChipPositionByIndex(3, 17);
	 player_->Initialize(&camera_,playerPos);
	 player_->SetMapChipField(mapChipField_);
	 player_->SetUI(ui_);
	  //enemy
	 // 3Dモデルの生成
	 enemymodel_ = KamataEngine::Model::CreateFromOBJ("Enemy", true);
	 bossmodel_ = KamataEngine::Model::CreateFromOBJ("cube", true);

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
   
    
    

    if (input_->TriggerKey(DIK_L)) {
        isGetExp = true;
    } 
    if (isGetExp) {
        exp = 250;
        isGetExp = false;
    } else {
        exp = 0;
    }

    ui_->Update(exp);

    // **暂停游戏：如果 UI 处于打开状态，停止游戏逻辑**
    if (ui_->IsUIOpen()) {
        isGamePaused = true;
        return;  // **跳出 Update()，游戏暂停**
    } else {
        isGamePaused = false;  // **UI 关闭后，恢复游戏**
    }

    // **当 UI 关闭时，应用玩家的武器选择**
    int selectedWeapon = ui_->GetSelectedWeapon();
    switch (selectedWeapon) {
        case 0:
            player_->SetBulletType(BulletType::Accelerating);
            break;
        case 1:
            player_->SetBulletType(BulletType::Spread);
            break;
        case 2:
            player_->SetBulletType(BulletType::TripleShot);
            break;
        case 3:
            player_->SetBulletType(BulletType::Orbit);
            break;
        default:
            player_->SetBulletType(BulletType::Normal);
            break;
    }

    // **如果游戏未暂停，才继续更新**
	timer_->Update();
    CheckAllcollisiions();

#ifdef _DEBUG
    if (input_->TriggerKey(DIK_SPACE)) {
        isDebugCameraActrive_ = !isDebugCameraActrive_;
    }
#endif 

    if (isDebugCameraActrive_) {
        debugCamera_->Update();
        camera_.matView = debugCamera_->GetCamera().matView;
        camera_.matProjection = debugCamera_->GetCamera().matProjection;
        camera_.TransferMatrix();
    } else {
        camera_.UpdateMatrix();
    }

    // **更新场景**
    for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
        for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
            if (!worldTransformBlock) continue;
            worldTransformBlock->UpdateMatrix();
        }
    }

    player_->Update();
     UpdateEnemySpawn();

    for (Enemy* enemy : enemys_) {
        enemy->Update();
    }

    enemys_.remove_if([this](Enemy* enemy) {
        if (enemy->IsDead()) {
            delete enemy;
            return true;
        }
        return false;
    });

	for (Item* item : items_) {
		item->Update();
	}
	// 移除已拾取的道具
items_.remove_if([](Item* item) {
    if (item->IsCollected()) {
        delete item;
        return true;
    }
    return false;
});
    cameraController_->Update();
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

	for (Item* item : items_) {
		item->Draw(camera_);
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
	timer_->Draw();
	ui_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
void GameScene::UpdateEnemySpawn() {
	static float spawnTimer = 0.0f;         // 生成普通敌人的计时器
	static float bossSpawnTimer = 0.0f;     // 生成 Boss 的计时器
	static float spawnInterval = 3.0f;      // 初始普通敌人生成间隔（秒）
	static int enemyCount = 3;              // 初始每次生成的普通敌人数量
	static float bossSpawnInterval = 30.0f; // Boss 生成的间隔（秒）
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

void GameScene::CheckAllcollisiions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	 // 判定玩家和敌人的碰撞
    Vector3 playerPos = player_->GetWorldPosition();

	for (Enemy* enemy : enemys_) {
		Vector3 enemyPos = enemy->GetWorldPosition();

		// 计算距离
		float length = KamataEngine::MathUtility::Length(playerPos - enemyPos);

		 // 检测是 Boss 还是普通敌人
        Boss* boss = dynamic_cast<Boss*>(enemy);
        float radius = boss ? boss->GetBossRadius() + Playerradius_ : Enemyradius_ + Playerradius_;

		// 如果碰撞
		if (length <= radius) {
			player_->TakeDamage(10);

			// 如果玩家HP <= 0，可以触发死亡逻辑
			if (HP <= 0) {
				// 这里可以添加游戏结束逻辑

			}
		}
	}
	//自弾リストの取得
	const std::list<BaseBullet*>& playerBullets = player_->GetBullets();
	const std::list<OrbitBullet*>& orbitBullets = player_->GetOrbitBullets();
		#pragma region 自弾と敵キャラの当たり判定
	for (Enemy* enemy : enemys_) {
		for (BaseBullet* bullet : playerBullets) {
			// 敵キャラの座標
			posA = enemy->GetWorldPosition();
			// 自弾の座標
			posB = bullet->GetWorldPosition();
			// 衝突判定
			float length = KamataEngine::MathUtility::Length(posB - posA);
		  Boss* boss = dynamic_cast<Boss*>(enemy);
        float radius = boss ? boss->GetBossRadius() + PlayerBulletradius_ : Enemyradius_ + PlayerBulletradius_;
			if (length <= radius) {
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				   // **如果是 Boss，调用受击方法**
				if (boss) {
					boss->TakeDamage(20);
				}
			}
		}
		for (OrbitBullet* orbitBullet : orbitBullets) {
			// 敵キャラの座標
			posA = enemy->GetWorldPosition();
			// 自弾の座標
			posB = orbitBullet->GetWorldPosition();
			// 衝突判定
			float length = KamataEngine::MathUtility::Length(posB - posA);
			  Boss* boss = dynamic_cast<Boss*>(enemy);
        float radius = boss ? boss->GetBossRadius() + PlayerBulletradius_ : Enemyradius_ + PlayerBulletradius_;
			if (length <= radius) {
				// 自弾の衝突時コールバックを呼び出す
				orbitBullet->OnCollision();
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				   // **如果是 Boss，调用受击方法**
				if (boss) {
					boss->TakeDamage(20);
				}
			}
		}
	}
	// 判定玩家与道具的碰撞
for (Item* item : items_) {
    if (item->IsCollected()) continue;  // 已拾取的道具跳过

    float distance = KamataEngine::MathUtility::Length(player_->GetWorldPosition() - item->GetWorldPosition());
    if (distance <= 2.0f) {  // 设定拾取范围
        item->Collect();
        ui_->Update(250);  // 增加经验值100
    }
}
	#pragma endregion 
}

void GameScene::DropItem(const KamataEngine::Vector3& position, bool isBoss) {
	int dropChance = rand() % 100;   // 生成 0-99 之间的随机数
	int dropRate = isBoss ? 60 : 30; // Boss 掉落率 60%，小型敌人 30%

	if (dropChance < dropRate) {
		if (dropChance < dropRate) {
			Item* newItem = new Item();
			newItem->Initialize(bossmodel_, position); // 使用 enemy 的模型，后续可更换
			newItem->SetGameScene(this);
			items_.push_back(newItem);
		}
	}
}