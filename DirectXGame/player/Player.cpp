#include "Player.h"
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
	for (BaseBullet* bullet : bullets_) {
		delete bullet;
	};
    for (OrbitBullet* bullet : orbitBullets_) {
		delete bullet;
	};
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
    bullets_.remove_if([](BaseBullet* bullet) {
        if (bullet->IsDead()) {
            delete bullet; 
            return true; 
        }
        return false;
    });
     orbitBullets_.remove_if([](OrbitBullet* bullet) {
        if (bullet->IsDead()) {
            delete bullet; 
            return true; 
        }
        return false;
    });
      // 检测子弹类型是否切换
    if (bulletType_ != previousBulletType_) {
        if (IsOrbitBulletType(previousBulletType_) && IsOrbitBulletType(bulletType_)) {
            // 如果旧的和新的子弹类型都是环绕子弹类型，则清除旧的环绕子弹
            for (OrbitBullet* bullet : orbitBullets_) {
                delete bullet;
            }
            orbitBullets_.clear();
        }
        previousBulletType_ = bulletType_; // 更新记录
    }
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
     // 地面检测
    CheckGroundCollision();
     // 处理击退效果
    if (isKnockback_) {
        CollisionMapInfo knockbackInfo;
        knockbackInfo.move = knockbackVelocity_; // 使用击退速度作为移动量
        MapCollision(knockbackInfo);             // 强制检测击退方向的地图碰撞
        worldTransform_.translation_ += knockbackInfo.move; // 应用安全的移动量
        
        // 击退时的小跳跃改为Z轴
        verticalVelocity_ = 0.3f; // 向上速度
        
        // 衰减击退速度
        knockbackVelocity_ *= knockbackDecay_;
        
        if (knockbackVelocity_.Length() < 0.01f && isGrounded_) {
            isKnockback_ = false;
            knockbackVelocity_ = Vector3{0,0,0};
        }
    }
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
     if (invincibleTime > 0.0f) {
        invincibleTime -= 1.0f / 60.0f; // 每帧减少 (假设游戏帧率是 60)
        if (invincibleTime < 0.0f) {
            invincibleTime = 0.0f;
        }
    }

	fireTimer_--; // 计时器递减
    if (fireTimer_ <= 0) {
        Attack();  // 自动开火
        fireTimer_ = (bulletType_ == BulletType::Accelerating||bulletType_ == BulletType::AcceleratingTripleShot||bulletType_ == BulletType::AcceleratingSpread) ? 30 : fireRate_; // 重新设置射击间隔
    }
	for(BaseBullet* bullet : bullets_) {
		bullet->Update();
	}
    for(OrbitBullet* bullet : orbitBullets_) {
         bullet->SetBulletCount(static_cast<int>(orbitBullets_.size()));
		bullet->Update();
	}
    if (input_->IsTriggerMouse(0)) {
        for (OrbitBullet* bullet : orbitBullets_) {
             // 计算当前子弹的方向，沿切线方向发射
        float bulletAngle = bullet->GetAngle();
        KamataEngine::Vector3 bulletVelocity{
            -sin(bulletAngle) * bulletSpeed_,
             cos(bulletAngle) * bulletSpeed_,
            0.0f
        };

        bullet->SetVelocity(bulletVelocity);
        bullet->SetOrbiting(false); // 让子弹从环绕状态变为发射状态
        }
    }
  if (bulletType_ != BulletType::Orbit&& bulletType_ != BulletType::SpreadOrbit&& bulletType_ != BulletType::TripleShotOrbit&&bulletType_ != BulletType::AcceleratingOrbit) {
        for (OrbitBullet* bullet : orbitBullets_) {
            delete bullet;
        }
        orbitBullets_.clear();
    }
    // 碰撞检测
    CollisionMapInfo collisionMapInfo;
    collisionMapInfo.move = velocity_;
    MapCollision(collisionMapInfo);

    // 更新位置
    worldTransform_.translation_ += collisionMapInfo.move;

    // 更新变换矩阵
    worldTransform_.UpdateMatrix();

    ShowImGuiControls(); 
}
void Player::CheckGroundCollision() {
    // 向下发射射线检测地面
    Vector3 rayStart = GetWorldPosition();
    Vector3 rayEnd = rayStart + Vector3{0, 0, -kGroundCheckDistance};
    
    // 简单版地面检测（实际项目中应该使用物理引擎或更精确的检测）
    isGrounded_ = (worldTransform_.translation_.z <= 0.0f);
    
    // 更精确的检测可以这样：
    // isGrounded_ = mapChipField_->CheckCollision(rayStart, rayEnd);
}

void Player::Draw()
{

    model_->Draw(worldTransform_, *camera_);
    for (BaseBullet* bullet : bullets_) {
        bullet->Draw(*camera_);
    }
    for (OrbitBullet* bullet : orbitBullets_) {
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

void Player::Attack() {
    std::vector<BaseBullet*> newBullets;
    std::vector<OrbitBullet*> newBulletsO;

  // 获取鼠标位置
    Vector2 mousePos = Input::GetInstance()->GetMousePosition();
    Vector3 worldPos = GetWorldPosition();

    // 计算鼠标相对玩家的位置
    float dx = mousePos.x - (SCREEN_WIDTH / 2.0f);
    float dy = (SCREEN_HEIGHT / 2.0f) - mousePos.y;

    // 计算单位方向向量
    float length = sqrt(dx * dx + dy * dy);
    KamataEngine::Vector3 direction = { dx / length, dy / length, 0.0f }; // 归一化向量

    // 计算子弹速度（沿鼠标方向）
    KamataEngine::Vector3 velocity = {
        direction.x * bulletSpeed_,
        direction.y * bulletSpeed_,
        direction.z * bulletSpeed_
    };

if (bulletType_ == BulletType::SpreadOrbit || bulletType_ == BulletType::TripleShotOrbit|| bulletType_ == BulletType::Orbit||bulletType_ == BulletType::AcceleratingOrbit) {
    orbitBulletCount_ = (bulletType_ == BulletType::SpreadOrbit) ? 8 : 4;
    if (orbitBullets_.empty()) {  
        newBulletsO = BulletFactory::CreateBullet(bulletType_, model_, &worldTransform_.translation_, orbitBulletCount_);
        for (OrbitBullet* bullet : newBulletsO) {
            orbitBullets_.push_back(bullet);
        }
    }
}
 else if (bulletType_ == BulletType::Accelerating||bulletType_ == BulletType::AcceleratingTripleShot||bulletType_ == BulletType::AcceleratingSpread) {
        KamataEngine::Vector3 accel(
            cos(worldTransform_.rotation_.z) * acceleration_,
            sin(worldTransform_.rotation_.z) * acceleration_,
            0
        );
        newBullets = BulletFactory::CreateBullet(bulletType_, model_, &worldTransform_.translation_, velocity, worldTransform_.rotation_.z, accel);
    } else {
        newBullets = BulletFactory::CreateBullet(bulletType_, model_,  &worldTransform_.translation_, velocity, worldTransform_.rotation_.z);
    }

    for (BaseBullet* bullet : newBullets) {
        bullets_.push_back(bullet);
    }
}

void Player::TakeDamage(float damage, const Vector3& attackerPosition)
{
     if (invincibleTime > 0.0f) {
        return;
    }

    HP -= damage;
    invincibleTime = invincibleDuration;

    // 计算击退方向并标准化
    Vector3 knockbackDirection = GetWorldPosition() - attackerPosition;
    knockbackDirection = knockbackDirection.Normalized(); // 使用成员函数
    
    // 应用击退速度
    knockbackVelocity_ = knockbackDirection * 0.5f; // 现在这个乘法可以工作了
    verticalVelocity_ = 0.3f;
    isKnockback_ = true;
    isGrounded_ = false;
    if (ui_) {
        ui_->SetCurrentHP(HP);
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

void Player::ShowImGuiControls() {
    ImGui::Begin("Bullet Controls"); // 开始 UI 窗口

    // **当前武器类型**
    const char* bulletTypes[] = {
        "Normal", "Spread", "TripleShot", "Orbit", "Accelerating",
        "SpreadTripleShot", "AcceleratingTripleShot", "AcceleratingOrbit",
        "AcceleratingSpread", "SpreadOrbit", "TripleShotOrbit"
    };
    
    int currentBulletType = static_cast<int>(bulletType_);

    // **只读显示当前武器类型**
    ImGui::Text("Current Weapon: %s", bulletTypes[currentBulletType]);

    // **允许更改武器**
    if (ImGui::Combo("Bullet Type", &currentBulletType, bulletTypes, IM_ARRAYSIZE(bulletTypes))) {
        bulletType_ = static_cast<BulletType>(currentBulletType);
    }

    // **其他参数调整**
    ImGui::SliderInt("Fire Rate (frames)", &fireRate_, 10, 120);
    ImGui::SliderFloat("Bullet Speed", &bulletSpeed_, 0.5f, 5.0f);

    if (bulletType_ == BulletType::Accelerating) {
        ImGui::SliderFloat("Acceleration", &acceleration_, 0.01f, 0.1f);
    }

    if (bulletType_ == BulletType::Orbit||bulletType_ == BulletType::TripleShotOrbit||bulletType_ == BulletType::SpreadOrbit||bulletType_ == BulletType::AcceleratingOrbit) {
        ImGui::SliderInt("Orbit Bullet Count", &orbitBulletCount_, 2, 10);
    }

    ImGui::End(); // 结束 UI 窗口
}
bool Player::IsOrbitBulletType(BulletType type) {
    return type == BulletType::Orbit ||
           type == BulletType::SpreadOrbit ||
           type == BulletType::AcceleratingOrbit||
           type == BulletType::TripleShotOrbit;
}
