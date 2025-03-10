#include "GameScene.h"
#include <cassert>
#include <cmath> 
#include <fstream>
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
	 enemymodel_ = KamataEngine::Model::CreateFromOBJ("Enemy", true);
	 bossmodel_ = KamataEngine::Model::CreateFromOBJ("cube", true);
	 LoadEnemyPopData();

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
		CheckAllcollisiions();
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
	UpdateEnemyPopCommands();
		// 敵の更新
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
void GameScene::EnemyPop(KamataEngine::Vector3 position, const std::string& type) {

	//KamataEngine::Vector3 spawnPosition;

	Enemy* newEnemy = nullptr;

	if (type == "Boss") {
		newEnemy = new Boss(); // 如果类型是 Boss，则创建 Boss 对象
	} else {
		newEnemy = new Enemy(); // 否则创建普通敌人
	}

	// 敵の生成

	// 敵キャラに自キャラのアドレスを渡す
	// newEnemy->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	newEnemy->SetGameScene(this);
	// 敵の初期化
	if (type == "Boss") {
		newEnemy->Initialize(bossmodel_,position); // 如果类型是 Boss，则创建 Boss 对象
	} else {
		newEnemy->Initialize(enemymodel_,position); // 否则创建普通敌人
	}
	if (player_) {
		newEnemy->SetPlayer(player_); // 传入玩家对象
	}
	enemys_.push_back(newEnemy);
	// 让 enemy_ 指向新创建的敌人（仅用于调试单个敌人）

}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources./enemyPop.csv");
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();
	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (waitFlag) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			waitFlag = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line;
	// コマンド実行ループ
	while (std::getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		std::string word;
		//,区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			// y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			// z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			std::string type;
			std::getline(line_stream, type, ','); // 新增解析类型列
			// 敵を発生させる
			EnemyPop(KamataEngine::Vector3(x, y, z), type);
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');
			// 待ち時間
			int32_t waitTime = atoi(word.c_str());
			// 待機時間
			waitFlag = true;
			waitTimer = waitTime;
			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::CheckAllcollisiions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

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
			float radius = PlayerBulletradius_ + Enemyradius_;
			if (length <= radius) {
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				   // **如果是 Boss，调用受击方法**
                Boss* boss = dynamic_cast<Boss*>(enemy);
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
			float radius = PlayerBulletradius_ + Enemyradius_;
			if (length <= radius) {
				// 自弾の衝突時コールバックを呼び出す
				orbitBullet->OnCollision();
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				   // **如果是 Boss，调用受击方法**
                Boss* boss = dynamic_cast<Boss*>(enemy);
				if (boss) {
					boss->TakeDamage(20);
				}
			}
		}
	}
	#pragma endregion 
}
