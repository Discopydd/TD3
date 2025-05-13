#include "GameScene.h"
#include <cassert>
#include <cmath> 
#include <fstream>
#include <cstdlib> 
#include "2d/DebugText.h"
void GameScene::ResetEnemySpawnParameters()
{
	spawnTimer_ = 0.0f;
    bossSpawnTimer_ = 0.0f;
    spawnInterval_ = 10.0f;
    enemyCount_ = 5;
    bossSpawnInterval_ = 30.0f;
    maxBossCount_ = 2;
	//static float spawnTimer = 0.0f;         // 生成普通敌人的计时器
	//static float bossSpawnTimer = 0.0f;     // 生成 Boss 的计时器
	//static float spawnInterval = 10.0f;      // 初始普通敌人生成间隔（秒）
	//static int enemyCount = 5;              // 初始每次生成的普通敌人数量
	//static float bossSpawnInterval = 30.0f; // Boss 生成的间隔（秒）
	//static int maxBossCount = 2;            // 限制最多同时存在的 Boss 数量
}
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
GameScene::GameScene() {
srand(static_cast<unsigned int>(time(nullptr)));
ResetEnemySpawnParameters();
}

GameScene::~GameScene() {


	delete model_;
	delete debugCamera_;
	delete enemymodel_;
	delete bossmodel_;
	delete itemmodel_;
	delete modelField_;
	delete player_;
	delete cameraController_;
	delete timer_;
	delete ui_;
	delete crystal_;
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
		   for (DeathParticles* deathParticlesList : deathParticlesList_) {
		    delete deathParticlesList;
	    }
	    deathParticlesList_.clear();
	    delete cursorSprite;
	    delete field_;

		audio_->StopWave(bgmVoiceHandle_);
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	win = KamataEngine::WinApp::GetInstance();

	model_ = Model::Create();
	camera_.Initialize();
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	 timer_ = new Timer();
	 timer_->Initialize();
	 timer_->SetTimeLemit(180.0f);
	 timer_->SetTriggerTime(30.0f);
	 timer_->SetEnemyPwerUpTime(12.0f);

	 crystal_ = new Crystal();
	 crystal_->Initialize();

	 ui_ = new PlayUI();
	 ui_->Initialize(HP, input_, crystal_, timer_);

	 //Map
	 mapChipField_ = new MapChipField;
	 mapChipField_->LoadMapChipCsv("Resources/map0.csv");
	 GenerateBlocks();

	 //Player
	player_ = new Player();
	Vector3 playerPos = mapChipField_->GetMapChipPositionByIndex(3, 17);
	 player_->Initialize(&camera_,playerPos);
	 player_->SetMapChipField(mapChipField_);
	 player_->SetUI(ui_);
	  //enemy
	 ResetEnemySpawnParameters();
	 // 3Dモデルの生成
	 enemymodel_ = KamataEngine::Model::CreateFromOBJ("Enemy", true);
	 bossmodel_ = KamataEngine::Model::CreateFromOBJ("boss", true);
	 itemmodel_ = KamataEngine::Model::CreateFromOBJ("Item", true);
    modelField_ = Model::CreateFromOBJ("field", true);
	 //

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

	cursorTexture = KamataEngine::TextureManager::Load("cursor.png");
	cursorSprite = KamataEngine::Sprite::Create(cursorTexture, {0.0f, 0.0f});
	cursorSprite->SetSize({32.0f, 32.0f});

	field_ = new Field();
	field_->Initialize(modelField_);

	timeGetSEDataHandle_ = audio_->LoadWave("Audio/EXPGet.wav");
	bgmDataHandle_ = audio_->LoadWave("Audio/gameBgm.wav");
	clearDataHandle_ = audio_->LoadWave("Audio/clear.wav");
	gameOverDataHandle_ = audio_->LoadWave("gameover.wav");
	bgmVoiceHandle_ = audio_->PlayWave(bgmDataHandle_, true, 0.55f);
}

void GameScene::Update() {
	// マウス位置取得
	KamataEngine::Vector2 mousePos = input_->GetMousePosition();
	pos.x = mousePos.x - 11;
	pos.y = mousePos.y - 10;
	cursorSprite->SetPosition(pos);
	ChangePhase();

	if (!firstUpdateDone) {
        firstUpdateDone = true;  // 第一帧执行后，允许绘制
    }
	crystal_->Update();
    ui_->Update(exp);

	field_->Update();

    // **暂停游戏：如果 UI 处于打开状态，停止游戏逻辑**
    if (crystal_->IsUIOpen()) {
        isGamePaused = true;
        return;  // **跳出 Update()，游戏暂停**
    } else {
        isGamePaused = false;  // **UI 关闭后，恢复游戏**
    }

    // **当 UI 关闭时，应用玩家的武器选择**
	Crystal::FirstCrystal first = crystal_->HaveFirstCrystal();
	Crystal::SecondCrystal second = crystal_->HaveSecondCrystal();
	switch (first) {
	case Crystal::FirstCrystal::None:
		player_->SetBulletType(BulletType::Normal);
		break;
	case Crystal::FirstCrystal::Fire:
		if (crystal_->IsSecondCrystalGet()) {
			switch (second) {
			case Crystal::SecondCrystal::Ice:
				player_->SetBulletType(BulletType::TripleShotOrbit);
				break;
			case Crystal::SecondCrystal::Wind:
				player_->SetBulletType(BulletType::AcceleratingTripleShot);
				break;
			case Crystal::SecondCrystal::Soil:
				player_->SetBulletType(BulletType::SpreadTripleShot);
				break;
			}
		} else {
    		player_->SetBulletType(BulletType::TripleShot);
		}
		break;
	case Crystal::FirstCrystal::Ice:
		if (crystal_->IsSecondCrystalGet()) {
			switch (second) {
			case Crystal::SecondCrystal::Fire:
				player_->SetBulletType(BulletType::AcceleratingOrbit);
				break;
			case Crystal::SecondCrystal::Wind:
				player_->SetBulletType(BulletType::TripleShotOrbit);
				break;
			case Crystal::SecondCrystal::Soil:
				player_->SetBulletType(BulletType::SpreadOrbit);
				break;
			}
		} else {
			player_->SetBulletType(BulletType::Orbit);
		}
		break;
	case Crystal::FirstCrystal::Wind:
		if (crystal_->IsSecondCrystalGet()) {
			switch (second) {
			case Crystal::SecondCrystal::Fire:
				player_->SetBulletType(BulletType::AcceleratingTripleShot);
				break;
			case Crystal::SecondCrystal::Ice:
				player_->SetBulletType(BulletType::AcceleratingOrbit);
				break;
			case Crystal::SecondCrystal::Soil:
				player_->SetBulletType(BulletType::AcceleratingSpread);
				break;
			}
		} else {
			player_->SetBulletType(BulletType::Accelerating);
		}
		break;
	case Crystal::FirstCrystal::Soil:
		if (crystal_->IsSecondCrystalGet()) {
			switch (second) {
			case Crystal::SecondCrystal::Fire:
				player_->SetBulletType(BulletType::SpreadTripleShot);
				break;
			case Crystal::SecondCrystal::Ice:
				player_->SetBulletType(BulletType::SpreadOrbit);
				break;
			case Crystal::SecondCrystal::Wind:
				player_->SetBulletType(BulletType::AcceleratingSpread);
				break;
			}
		} else {
			player_->SetBulletType(BulletType::Spread);
		}
		break;
	}
	 if (crystal_->SelectStatus() != Crystal::StatusUP::None) {
        player_->ApplyStatusUp(crystal_->SelectStatus());
        crystal_->RestStatusSelect(); // ステータス選択をリセット
    }
    // **如果游戏未暂停，才继续更新**

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

	switch (phase) {
	case GameScene::Phase::Play:
		timer_->Update();
		 if (timer_->GetCurrentTime() > 0) {
           damageMultiplier_ = 1.0f + floor(timer_->GetCurrentTime() / 30.0f) * 0.2f;  // 每30秒增加0.2倍
			 hpMultiplier_ = 1.0f + floor(timer_->GetCurrentTime() / 30.0f) * 0.3f;     // 每30秒增加0.3倍
        }
		player_->Update();
		UpdateEnemySpawn();

		for (Enemy* enemy : enemys_) {
			enemy->Update();
		}

		enemys_.remove_if([this](Enemy* enemy) {
			if (enemy->IsDead()) {
				CreateDeathParticles(enemy->GetWorldPosition());
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
			if (item->IsFullyCollected()) {
				delete item;
				return true;
			}
			return false;
		});
		// パーティクルの更新
		for (auto it = deathParticlesList_.begin(); it != deathParticlesList_.end();) {
			(*it)->Update();
			if ((*it)->GetParticlesOver()) { // 结束的粒子删除
				delete *it;
				it = deathParticlesList_.erase(it);
			} else {
				++it;
			}
		}

		cameraController_->Update();
		break;
	case GameScene::Phase::GameCler:
		timer_->Update();
		for (Enemy* enemy : enemys_) {
			if (Boss* boss = dynamic_cast<Boss*>(enemy)) {
            boss->BossDead();
        } else {
            enemy->EnemyDead();
        }

		}
		enemys_.remove_if([this](Enemy* enemy) {
			if (enemy->IsDead()) {
				CreateDeathParticles(enemy->GetWorldPosition());
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
			if (item->IsFullyCollected()) {
				delete item;
				return true;
			}
			return false;
		});
		// パーティクルの更新
		for (auto it = deathParticlesList_.begin(); it != deathParticlesList_.end();) {
			(*it)->Update();
			if ((*it)->GetParticlesOver()) { // 结束的粒子删除
				delete *it;
				it = deathParticlesList_.erase(it);
			} else {
				++it;
			}
		}

		cameraController_->Update();
		break;
	case GameScene::Phase::GameOver:

		for (Item* item : items_) {
			item->Update();
		}
		// 移除已拾取的道具
		items_.remove_if([](Item* item) {
			  if (item->IsFullyCollected()) { 
        delete item;
        return true;
    }
    return false;
		});
		// パーティクルの更新
		for (auto it = deathParticlesList_.begin(); it != deathParticlesList_.end();) {
			(*it)->Update();
			if ((*it)->GetParticlesOver()) { // 结束的粒子删除
				delete *it;
				it = deathParticlesList_.erase(it);
			} else {
				++it;
			}
		}

		cameraController_->Update();
		break;
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

	field_->Draw(camera_);

	if (firstUpdateDone) {  // 确保第一帧不会绘制
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) continue;
				model_->Draw(*worldTransformBlock, camera_);
			}
		}
		player_->Draw();
		for (Enemy* enemy : enemys_) {
			enemy->Draw(camera_);
		}
		for (DeathParticles* particle : deathParticlesList_) {
    particle->Draw();
}

		for (Item* item : items_) {
			item->Draw(camera_);
		}
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
	if (firstUpdateDone) {
		timer_->Draw();
		ui_->Draw();
	}
	cursorSprite->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
void GameScene::UpdateEnemySpawn() {

	spawnTimer_ += 1.0f / 60.0f;     // 普通敌人计时
	bossSpawnTimer_ += 1.0f / 60.0f; // Boss 计时

	// 生成普通敌人
	if (spawnTimer_ >= spawnInterval_) {
		spawnTimer_ = 0.0f;
		for (int i = 0; i < enemyCount_; i++) {
			SpawnEnemyNearPlayer();
		}

		// 逐渐加快普通敌人生成速度 & 增加数量
		if (spawnInterval_ > 1.0f) {
			spawnInterval_ -= 0.1f;
		}
		enemyCount_++;
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
	if (rand() % 100 < 10) { // 20% 概率
		newEnemy = new Boss();
		float bossHP = static_cast<int>(500 * hpMultiplier_); // 基础HP500乘以倍率
		static_cast<Boss*>(newEnemy)->Initialize(bossmodel_, spawnPosition, bossHP);
	} else {
		newEnemy = new Enemy();
		float enemyHP = static_cast<int>(100 * hpMultiplier_); // 基础HP100乘以倍率
		newEnemy->Initialize(enemymodel_, spawnPosition , enemyHP);
	}

	newEnemy->SetGameScene(this);
	newEnemy->SetPlayer(player_);
	enemys_.push_back(newEnemy);
}



/// <summary>
/// /////////////////////////////////////////////////////////////////////////////////
/// </summary>
void GameScene::CheckAllcollisiions()
{

	 // 判定玩家和敌人的碰撞
    Vector3 playerPos = player_->GetWorldPosition();

	for (Enemy* enemy : enemys_) {
		if (enemy->IsSpawning()) continue;
		Vector3 enemyPos = enemy->GetWorldPosition();

		// 计算距离
		float length = KamataEngine::MathUtility::Length(playerPos - enemyPos);

		// 检测是 Boss 还是普通敌人
        Boss* boss = dynamic_cast<Boss*>(enemy);
        float radius = boss ? boss->GetBossRadius() + Playerradius_ : Enemyradius_ + Playerradius_;

		// 如果碰撞
		if (length <= radius) {
			float baseDamage = boss ? 20.0f : 10.0f;
            float scaledDamage = baseDamage * damageMultiplier_; 


			KamataEngine::Vector3 dir = myMath::Subtract(playerPos, enemyPos);
			enemy->StartAttack(dir);
			player_->TakeDamage(scaledDamage, enemyPos);
			// 如果玩家HP <= 0，可以触发死亡逻辑
			if (HP <= 0) {
			
			}
		}
	}
	//自弾リストの取得
	const std::list<BaseBullet*>& playerBullets = player_->GetBullets();
	const std::list<OrbitBullet*>& orbitBullets = player_->GetOrbitBullets();
		#pragma region 自弾と敵キャラの当たり判定
// 自弾と敵キャラの当たり判定
    for (BaseBullet* bullet : playerBullets) {
        if (bullet->IsDead() || bullet->HasHit()) continue; // 跳过已击中或已销毁的子弹

        for (Enemy* enemy : enemys_) {
            if (enemy->IsDead() || enemy->IsSpawning()) continue; // 跳过已死亡的敌人

            Vector3 posA = enemy->GetWorldPosition();
            Vector3 posB = bullet->GetWorldPosition();
            float length = KamataEngine::MathUtility::Length(posB - posA);

            Boss* boss = dynamic_cast<Boss*>(enemy);
            float radius = boss ? boss->GetBossRadius() + PlayerBulletradius_ : Enemyradius_ + PlayerBulletradius_;

            if (length <= radius) {
                // 子弹命中逻辑
                bullet->OnCollision();
                bullet->SetHit(true); // 标记子弹为已击中

				 // 统一调用 TakeDamage，传入基础伤害值
				float baseDamage = 100.0f;                            // 基础伤害值
				float scaledDamage = baseDamage * damageMultiplier_; // 应用时间倍率
				enemy->TakeDamage(scaledDamage);                     // 调用统一的 TakeDamage
              /*  enemy->OnCollision();

                if (boss) {
                    boss->TakeDamage(20);
                }*/
                break; // 子弹击中一个敌人后，跳过剩余敌人检测
            }
        }
    }

    // 同样的逻辑处理环绕子弹
    for (OrbitBullet* orbitBullet : orbitBullets) {
        if (orbitBullet->IsDead() || orbitBullet->HasHit()) continue;

        for (Enemy* enemy : enemys_) {
            if (enemy->IsDead() || enemy->IsSpawning()) continue;

            Vector3 posA = enemy->GetWorldPosition();
            Vector3 posB = orbitBullet->GetWorldPosition();
            float length = KamataEngine::MathUtility::Length(posB - posA);

            Boss* boss = dynamic_cast<Boss*>(enemy);
            float radius = boss ? boss->GetBossRadius() + PlayerBulletradius_ : Enemyradius_ + PlayerBulletradius_;

            if (length <= radius) {
                orbitBullet->OnCollision();
                enemy->OnCollision();

                if (boss) {
                    boss->TakeDamage(20);
                }
                break;
            }
        }
    }
	// 判定玩家与道具的碰撞
for (Item* item : items_) {
    if (item->IsCollected() || item->IsFullyCollected()) continue;

    Vector3 itemPos = item->GetWorldPosition();
    float distance = KamataEngine::MathUtility::Length(playerPos - itemPos);

    if (distance <= 2.0f) { // 拾取距离
		timeGetSEVoiceHandle_ = audio_->PlayWave(timeGetSEDataHandle_, false, 0.3f);
        item->Collect();
		ui_->Update(350);
    }
}
	#pragma endregion 
}


//////////////////////////////////////////////////


void GameScene::DropItem(const KamataEngine::Vector3& position, bool isBoss) {
	int dropChance = rand() % 100;   // 生成 0-99 之间的随机数
	int dropRate = isBoss ? 80 : 50; // Boss 掉落率 60%，小型敌人 30%

	if (dropChance < dropRate) {
		if (dropChance < dropRate) {
			Item* newItem = new Item();
			newItem->Initialize(itemmodel_, position); // 使用 enemy 的模型，后续可更换
			newItem->SetGameScene(this);
			items_.push_back(newItem);
		}
	}
}

void GameScene::CreateDeathParticles(const KamataEngine::Vector3& position)
{
	 DeathParticles* newParticle = new DeathParticles();
    newParticle->Initialize(&camera_);  // 初始化
    newParticle->SetStartPos(position); // 设定起始位置
    newParticle->SetIsStart(true);      // 设定为开始状态
    deathParticlesList_.push_back(newParticle); // 加入列表
}

bool GameScene::IsPlayerDead() const
{
	return player_ && player_->GetHP() <= 0; }

void GameScene::ChangePhase() {
	switch (phase) {
	case GameScene::Phase::Play:
		if (player_->GetHP() <= 0) {
			audio_->StopWave(bgmVoiceHandle_);
			phase = Phase::GameOver;
			ResetEnemySpawnParameters();
		} else if (timer_->IsTimeUp()) {
			audio_->StopWave(bgmVoiceHandle_);
			phase = Phase::GameCler;
			ResetEnemySpawnParameters();
		}
		break;
	case GameScene::Phase::GameCler:
		clearVoiceHandle_ = audio_->PlayWave(clearDataHandle_, false, 0.5f);
		if (ui_->GetAlpha() >= 1) {
			if (input_->TriggerKey(DIK_SPACE) || (crystal_->IsMouseInWindow(win->GetHwnd()) && input_->IsTriggerMouse(0))) {
				audio_->StopWave(clearVoiceHandle_);
				isFinished = true;
			}
		}
		break;
	case GameScene::Phase::GameOver:
		gameOverVoiceHandle_ = audio_->PlayWave(gameOverDataHandle_, false, 0.5f);
		if (ui_->GetAlpha() >= 1) {
			if (input_->TriggerKey(DIK_SPACE) || (crystal_->IsMouseInWindow(win->GetHwnd()) && input_->IsTriggerMouse(0))) {
				audio_->StopWave(gameOverVoiceHandle_);
				isFinished = true;
			}
		}
		break;
	}
}
