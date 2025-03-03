#include "Player.h"
#include "BulletBehaviors.h"
#include <imgui.h>


AABB Player::GetAABB()
{
    Vector3 worldPos = GetWorldPosition();
    AABB aabb;

    aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
    aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

    return aabb;
}

void Player::OnCollision()
{
}

Player::~Player() {
	delete model_;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();
    for (PlayerBullet* bullet : orbitBullets_) {
		delete bullet;
	}
	orbitBullets_.clear();
}

void Player::Initialize(Camera* camera, const Vector3& position)
{
    worldTransform_.Initialize();
    camera_ = camera;
    worldTransform_.translation_ = position;
    model_ = Model::CreateFromOBJ("Player", true);
	input_ = KamataEngine::Input::GetInstance();
}

void Player::Update() {
    bullets_.remove_if([](PlayerBullet* bullet) {
        if (bullet->IsDead()) {
            delete bullet;
            return true;
        }
        return false;
        });
    // 获取鼠标位置
    Vector2 mousePos = Input::GetInstance()->GetMousePosition();

    // 获取玩家的世界坐标
    Vector3 worldPos = GetWorldPosition();

    // 计算鼠标相对玩家的位置
    float dx = mousePos.x - (SCREEN_WIDTH / 2.0f); // 屏幕中心偏移
    float dy = (SCREEN_HEIGHT / 2.0f) - mousePos.y; // Y 轴方向通常是反的

    // 计算目标角度（鼠标朝向）
    float targetAngle = atan2(dy, dx); // 计算旋转角度，弧度制

    // 应用角度到玩家的旋转
    worldTransform_.rotation_.z = targetAngle;

    // 处理移动输入
    Vector3 acceleration{};
    if (input_->PushKey(DIK_D)) {
        acceleration.x += kAcceleration;
    }
    if (input_->PushKey(DIK_A)) {
        acceleration.x -= kAcceleration;
    }
    if (input_->PushKey(DIK_W)) {
        acceleration.y += kAcceleration;
    }
    if (input_->PushKey(DIK_S)) {
        acceleration.y -= kAcceleration;
    }

    // 速度更新
    velocity_ += acceleration;
    velocity_.x *= (1 - kAttenuation);
    velocity_.y *= (1 - kAttenuation);
    velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
    velocity_.y = std::clamp(velocity_.y, -kLimitRunSpeed, kLimitRunSpeed);


    if (input_->TriggerKey(DIK_1)) currentBulletType_ = BulletType::Normal;
    if (input_->TriggerKey(DIK_3)) currentBulletType_ = BulletType::Scatter;
    if (input_->TriggerKey(DIK_2)) currentBulletType_ = BulletType::Fast;
    if (input_->TriggerKey(DIK_4)) currentBulletType_ = BulletType::RapidFire;
    if (input_->TriggerKey(DIK_5)) currentBulletType_ = BulletType::ScatterFast;
    if (input_->TriggerKey(DIK_6)) {
        currentBulletType_ = BulletType::RapidScatter;
    }
     if (input_->TriggerKey(DIK_7)) {
        currentBulletType_ = BulletType::SurroundShot;
    }
    // **自动攻击逻辑**
    int adjustedFireRate = fireRate_;
    if (currentBulletType_ == BulletType::Scatter) {
        adjustedFireRate = static_cast<int>(fireRate_ * 1.8f);  // Scatter 子弹间隔
    }
    else if (currentBulletType_ == BulletType::Fast) {
        adjustedFireRate = static_cast<int>(fireRate_ * 0.5f);  // Fast 子弹射速加快
    }
    else if (currentBulletType_ == BulletType::RapidFire) {
        adjustedFireRate = static_cast<int>(fireRate_ * 1.8f);  // 
    }
    else if (currentBulletType_ == BulletType::ScatterFast) {
        adjustedFireRate = static_cast<int>(fireRate_ * 1.2f);
    }else if (currentBulletType_ == BulletType::RapidScatter) {
    adjustedFireRate = static_cast<int>(fireRate_ * 1.8f);
}
    AttackSingle();
    fireTimer_--; // 计时器递减
    if (fireTimer_ <= 0) {
        Attack();  // 自动开火
        fireTimer_ = adjustedFireRate; // 重新设置射击间隔
    }
    for (PlayerBullet* bullet : bullets_) {
        bullet->Update();
    }
    if (currentBulletType_ != BulletType::SurroundShot) {
        for (PlayerBullet* bullet : orbitBullets_) {
            delete bullet;
        }
        orbitBullets_.clear();
    }
    AttackSurround();
    // 碰撞检测
    CollisionMapInfo collisionMapInfo;
    collisionMapInfo.move = velocity_;
    MapCollision(collisionMapInfo);

    // 更新位置
    worldTransform_.translation_ += collisionMapInfo.move;

    // 更新变换矩阵
    worldTransform_.UpdateMatrix();

// **ImGui UI 控制**
    ImGui::Begin("Player Stats");
    ImGui::Text("Bullet Type: %s",
        currentBulletType_ == BulletType::Normal ? "Normal" :
        currentBulletType_ == BulletType::Scatter ? "Scatter" :
        currentBulletType_ == BulletType::Fast ? "Fast" :
        currentBulletType_ == BulletType::RapidFire ? "RapidFire" :
        currentBulletType_ == BulletType::ScatterFast ? "ScatterFast" :
        "RapidScatter");
    ImGui::SliderInt("Fire Rate", &fireRate_, 10, 120);
    ImGui::Text("Bullet Count: %d", static_cast<int>(bullets_.size()));
    ImGui::Text("Fire Timer: %d", fireTimer_);
    ImGui::SliderInt("Bullet Count", &orbitBulletCount_, 1, 12);
    ImGui::SliderFloat("Orbit Radius", &orbitRadius_, 1.0f, 10.0f);
    ImGui::SliderFloat("Orbit Speed", &orbitSpeed_, 0.01f, 0.20f);
    ImGui::End();
}


void Player::Draw()
{
   
    model_->Draw(worldTransform_, *camera_);
	for(PlayerBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}
     for (PlayerBullet* bullet : orbitBullets_) {
        bullet->Draw(*camera_);
    }
}

void Player::MapCollision(CollisionMapInfo& info) {
    MapCollision_Up(info);
    MapCollision_Down(info);
    MapCollision_Left(info);
    MapCollision_Right(info);
}

void Player::MapCollision_Up(CollisionMapInfo& info)
{ 
    if (info.move.y <= 0)
        return;
    std::array<Vector3, static_cast<int>(kNumCorner)> positionsNew{};
    for (uint32_t i = 0; i < positionsNew.size(); ++i) {
        positionsNew[i] = CornerPosition(worldTransform_.translation_ + Vector3(0, info.move.y, 0), static_cast<Corner>(i));
    }

   
    MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;
   indexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			indexSet = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + Vector3(0, info.move.y + 0.1f, 0) + Vector3(0, kHeight / 2.0f, 0));
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = max(0.0f, (rect.bottom - worldTransform_.translation_.y) - ((kHeight / 2.0f) + kBlank));
			info.ceiling = true;
		}
	}
}

void Player::MapCollision_Down(CollisionMapInfo& info)
{
    if (info.move.y >= 0) return;
    std::array<Vector3, static_cast<int>(kNumCorner)> positionsNew{};
    for (uint32_t i = 0; i < positionsNew.size(); ++i) {
        positionsNew[i] = CornerPosition(worldTransform_.translation_ + Vector3(0, info.move.y, 0), static_cast<Corner>(i));
    }

 	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	bool hit = false;
	MapChipField::IndexSet indexSet;
  indexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
		if (indexSetNow.yIndex != indexSet.yIndex) {
			indexSet = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = min(0.0f, (rect.top - worldTransform_.translation_.y) + ((kHeight / 2.0f) + kBlank));
			info.landing = true;
		}
	}
}

void Player::MapCollision_Left(CollisionMapInfo& info)
{
   std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + Vector3(info.move.x, 0, 0), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = max(0.0f, (rect.right - worldTransform_.translation_.x) - (kWidth / 2.0f + kBlank));
	}
}

    void Player::MapCollision_Right(CollisionMapInfo& info)
    {
       std::array<Vector3, kNumCorner> positionsNew;

	    for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		    positionsNew[i] = CornerPosition(worldTransform_.translation_ + Vector3(info.move.x, 0, 0), static_cast<Corner>(i));
	    }

	    MapChipType mapChipType;
	    bool hit = false;
	    MapChipField::IndexSet indexSet;
	    indexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
	    mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	    if (mapChipType == MapChipType::kBlock) {
		    hit = true;
	    }
	    indexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kRightTop]);
	    mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	    if (mapChipType == MapChipType::kBlock) {
		    hit = true;
	    }
	    if (hit) {
		    MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		    info.move.x = min(0.0f, (rect.left - worldTransform_.translation_.x) + (kWidth / 2.0f + kBlank));
	    }
    }

Vector3 Player::GetWorldPosition()
{
    Vector3 worldPos;
    worldPos.x = worldTransform_.translation_.x;
    worldPos.y = worldTransform_.translation_.y;
    worldPos.z = worldTransform_.translation_.z;
    return worldPos;
}

void Player::Attack()
{
     if (currentBulletType_ == BulletType::SurroundShot) {
        if (orbitBullets_.size() != orbitBulletCount_) { // 只有当数量不同才重新生成
            for (PlayerBullet* bullet : orbitBullets_) {
                delete bullet;
            }
            orbitBullets_.clear();

            for (int i = 0; i < orbitBulletCount_; ++i) {
                float angle = (2.0f * PI / orbitBulletCount_) * i;
                KamataEngine::Vector3 offset(
                    cos(angle) * orbitRadius_,
                    sin(angle) * orbitRadius_,
                    0);

                PlayerBullet* newBullet = new PlayerBullet();
                newBullet->Initialize(model_, GetWorldPosition() + offset, {0, 0, 0}, currentBulletType_);
                orbitBullets_.push_back(newBullet);
            }
        }
        return; // 避免生成普通子弹
    }

    float kBulletSpeed = 1.0f;
    float bulletAngle = worldTransform_.rotation_.z;

    KamataEngine::Vector3 velocity(
        cos(bulletAngle) * kBulletSpeed,
        sin(bulletAngle) * kBulletSpeed,
        0);
    // RapidFire 模式：初始化三连发
    if ((currentBulletType_ == BulletType::RapidFire || currentBulletType_ == BulletType::RapidScatter) &&  !isRapidFiring_) {
        isRapidFiring_ = true;
        rapidFireCount_ = 3;  // 需要发射 3 颗子弹
        rapidFireCooldown_ = 5; // 等待 5 帧后开始发射第一颗子弹
        return; // 直接返回，不立即发射
    }
    PlayerBullet* newBullet = new PlayerBullet();
    newBullet->Initialize(model_, GetWorldPosition(), velocity, currentBulletType_);
    newBullet->SetBulletList(bullets_);
    // 根据类型动态添加行为
    if (currentBulletType_ == BulletType::ScatterFast) {
        kBulletSpeed = 0.75f;
        newBullet->AddBehavior(std::make_unique<FastBehavior>());
        newBullet->AddBehavior(std::make_unique<ScatterBehavior>());
    }
    else if (currentBulletType_ == BulletType::Scatter) {
        kBulletSpeed = 0.25f;
        newBullet->AddBehavior(std::make_unique<ScatterBehavior>());
    }
    else if (currentBulletType_ == BulletType::Fast) {
        kBulletSpeed = 1.5f;
        newBullet->AddBehavior(std::make_unique<FastBehavior>());
    }
    else if (currentBulletType_ == BulletType::RapidFire) {
        newBullet->AddBehavior(std::make_unique<RapidFireBehavior>());
    } 
    else if (currentBulletType_ == BulletType::RapidScatter) {
        newBullet->AddBehavior(std::make_unique<ScatterBehavior>());
    }

    bullets_.push_back(newBullet);
}

void Player::AttackSingle()
{
       // 处理三连发逻辑

if ((currentBulletType_ == BulletType::RapidFire ||currentBulletType_ == BulletType::RapidScatter) && isRapidFiring_) {
    if (rapidFireCooldown_ > 0) {
        rapidFireCooldown_--; // 计时器递减
    } 
    else if (rapidFireCount_ > 0) { 
        Attack();  // 发射下一颗子弹
        rapidFireCooldown_ = 3; // 设定 5 帧间隔
        rapidFireCount_--;

        if (rapidFireCount_ == 0) {
            isRapidFiring_ = false; // 三连发结束
            fireTimer_ = fireRate_; // 重新进入正常射击间隔
        }
    }
}
}

void Player::AttackSurround()
{
     if (currentBulletType_ == BulletType::SurroundShot) {
        orbitAngle_ += orbitSpeed_; 
        for (int i = 0; i < orbitBullets_.size(); ++i) {
            float angle = (2.0f * PI / orbitBullets_.size()) * i + orbitAngle_;
            KamataEngine::Vector3 offset(
                cos(angle) * orbitRadius_,
                sin(angle) * orbitRadius_,
                0);
            orbitBullets_[i]->SetPosition(GetWorldPosition() + offset);
        }
    }
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner)
{
    Vector3 offsetTable[kNumCorner] = {
        {+kWidth / 2.0f, -kHeight / 2.0f, 0.0f},  // kRightBottom
        {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f},  // kLeftBottom
        {+kWidth / 2.0f, +kHeight / 2.0f, 0.0f},  // kRightTop
        {-kWidth / 2.0f, +kHeight / 2.0f, 0.0f}   // kLeftTop
    };
    return center + offsetTable[static_cast<uint32_t>(corner)];
}