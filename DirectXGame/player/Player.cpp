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
     // 初始化平滑伤害相关变量
    pendingDamage_ = 0.0f;
    damagePerFrame_ = 0.0f;
    isTakingDamage_ = false;
    HP = 100.0f; // 确保初始HP正确
    invincibleTime = 0.0f; // 初始化无敌时间为0

    // 初始化子弹相关
    bulletType_ = BulletType::Normal;
    previousBulletType_ = BulletType::Normal;
    fireRate_ = 60;
    fireTimer_ = 0;
    bulletSpeed_ = 1.0f;
    acceleration_ = 0.04f;
    orbitBulletCount_ = 0;

    // 初始化移动相关
    velocity_ = Vector3{0, 0, 0};
    knockbackVelocity_ = Vector3{0, 0, 0};
    isKnockback_ = false;
    isGrounded_ = false;
    verticalVelocity_ = 0.0f;
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
     // 处理平滑伤害
    if (isTakingDamage_ && pendingDamage_ > 0.0f) {
        float damageThisFrame = min(damagePerFrame_, pendingDamage_);
        HP -= damageThisFrame;
        pendingDamage_ -= damageThisFrame;

        if (ui_) {
            ui_->SetCurrentHP(HP); // 更新UI显示
        }

        if (pendingDamage_ <= 0.0f) {
            isTakingDamage_ = false;
        }
    }
   
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
     // 处理重力
    if (!isGrounded_) {
        verticalVelocity_ += kGravity; // 重力加速度
        worldTransform_.translation_.z += verticalVelocity_;
    }

    // 地面检测
    CheckGroundCollision();

    // 确保玩家不会低于地面
    if (worldTransform_.translation_.z < 0.0f) {
        worldTransform_.translation_.z = 0.0f;
        verticalVelocity_ = 0.0f;
        isGrounded_ = true;
    }
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
const float currentMaxSpeed = kLimitRunSpeed * moveSpeedMultiplier_;
velocity_.x = std::clamp(velocity_.x, -currentMaxSpeed, currentMaxSpeed);
velocity_.y = std::clamp(velocity_.y, -currentMaxSpeed, currentMaxSpeed);
     if (invincibleTime > 0.0f) {
        invincibleTime -= 1.0f / 60.0f; // 每帧减少 (假设游戏帧率是 60)
        if (invincibleTime < 0.0f) {
            invincibleTime = 0.0f;
        }
    }
      if (tripleShotCounter_ > 0) {
        tripleShotTimer_--;
        if (tripleShotTimer_ <= 0) {
            // 生成单个子弹
            std::vector<BaseBullet*> newBullets;
            float length = sqrt(dx * dx + dy * dy);
             float rotation = atan2(dy, dx);
            KamataEngine::Vector3 direction = { dx / length, dy / length, 0.0f };
            KamataEngine::Vector3 velocity = direction * bulletSpeed_;

            // 创建单个子弹
            if (bulletType_ == BulletType::TripleShot) {
                newBullets = BulletFactory::CreateBullet(BulletType::Normal, model_, &worldPos, velocity, worldTransform_.rotation_.z);
            }
            else if (bulletType_ == BulletType::AcceleratingTripleShot) {
                KamataEngine::Vector3 accel(
                    cos(worldTransform_.rotation_.z) * acceleration_,
                    sin(worldTransform_.rotation_.z) * acceleration_,
                    0
                );
                newBullets = BulletFactory::CreateBullet(BulletType::Accelerating, model_, &worldPos, velocity, worldTransform_.rotation_.z, accel);
            }
            if (bulletType_ == BulletType::SpreadTripleShot) {
                newBullets = BulletFactory::CreateBullet(BulletType::Spread, model_, &worldPos, velocity, rotation);
            }
            // 添加子弹
            for (BaseBullet* bullet : newBullets) {
                bullets_.push_back(bullet);
            }

            tripleShotCounter_--;
            tripleShotTimer_ = kTripleShotInterval;
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
    // 如果是三连发类型且正在发射中，则等待发射完成
    if (IsTripleShotType(bulletType_) && tripleShotCounter_ > 0) {
        return;
    }

    std::vector<BaseBullet*> newBullets;
    std::vector<OrbitBullet*> newBulletsO;

    // 获取鼠标方向（原逻辑）
    Vector2 mousePos = Input::GetInstance()->GetMousePosition();
    Vector3 worldPos = GetWorldPosition();
    float dx = mousePos.x - (SCREEN_WIDTH / 2.0f);
    float dy = (SCREEN_HEIGHT / 2.0f) - mousePos.y;
    float length = sqrt(dx * dx + dy * dy);
    KamataEngine::Vector3 direction = { dx / length, dy / length, 0.0f };
    KamataEngine::Vector3 velocity = direction * bulletSpeed_;

    // 处理三连发子弹
    if (IsTripleShotType(bulletType_)) {
        // 重置计数器
        tripleShotCounter_ = 3;
        tripleShotTimer_ = kTripleShotInterval;
    }

    // 其他子弹类型（原逻辑）
    if (IsOrbitBulletType(bulletType_)) {
        orbitBulletCount_ = (bulletType_ == BulletType::SpreadOrbit) ? 8 : 4;
        if (orbitBullets_.empty()) {  
            newBulletsO = BulletFactory::CreateBullet(bulletType_, model_, &worldTransform_.translation_, orbitBulletCount_);
            for (OrbitBullet* bullet : newBulletsO) {
                orbitBullets_.push_back(bullet);
            }
        }
    }
    else if (bulletType_ == BulletType::Accelerating || bulletType_ == BulletType::AcceleratingTripleShot || bulletType_ == BulletType::AcceleratingSpread) {
        KamataEngine::Vector3 accel(
            cos(worldTransform_.rotation_.z) * acceleration_,
            sin(worldTransform_.rotation_.z) * acceleration_,
            0
        );
        newBullets = BulletFactory::CreateBullet(bulletType_, model_, &worldTransform_.translation_, velocity, worldTransform_.rotation_.z, accel);
    }
    else {
        newBullets = BulletFactory::CreateBullet(bulletType_, model_, &worldTransform_.translation_, velocity, worldTransform_.rotation_.z);
    }

    // 添加子弹到列表
    for (BaseBullet* bullet : newBullets) {
        bullets_.push_back(bullet);
    }
}

void Player::TakeDamage(float damage, const Vector3& attackerPosition)
{
     if (invincibleTime > 0.0f) {
        return;
    }

  // 防御力によるダメージ軽減を適用
    float actualDamage = damage * defenseMultiplier_;
    pendingDamage_ += actualDamage;
    damagePerFrame_ = pendingDamage_ / (damageDuration_ * 60.0f);
    isTakingDamage_ = true;
    invincibleTime = invincibleDuration;

    // 计算击退方向并标准化
    Vector3 knockbackDirection = GetWorldPosition() - attackerPosition;
    knockbackDirection.z = 0.0f;
    knockbackDirection = knockbackDirection.Normalized(); // 使用成员函数
    
    // 应用击退速度（仅水平方向）
    knockbackVelocity_ = knockbackDirection * 0.5f;
    isKnockback_ = true;
  
}

void Player::ApplyStatusUp(Crystal::StatusUP status) {
    switch (status) {
    case Crystal::StatusUP::Power:
        attackPowerMultiplier_ += 0.5f; // 攻撃力アップ
        break;
    case Crystal::StatusUP::Hp:
        baseMaxHP_ += 50.0f; // HP+50
        HP = baseMaxHP_; // HPを全回復
       if (ui_) {
                ui_->SetMaxHP(baseMaxHP_); // 更新 UI 的最大 HP
                ui_->SetCurrentHP(HP); // 更新当前 HP 显示
            }
        break;
    case Crystal::StatusUP::Defense:
        defenseMultiplier_ = max(0.5f, defenseMultiplier_ - 0.1f); // ダメージ10%減 (最小50%)
        break;
    case Crystal::StatusUP::Speed:
        moveSpeedMultiplier_ += 0.50f; // 移動速度50%アップ
        break;
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
 
    // ▼▼▼ 基本情報 ▼▼▼
    ImGui::Text("Current HP: %.1f / %.1f", HP, GetMaxHP());
    ImGui::ProgressBar(HP / GetMaxHP(), ImVec2(-1, 20));
    
    // ▼▼▼ ステータス数値 ▼▼▼
    ImGui::Separator();
    ImGui::Text("Attack Power: %.1f (Base: %.1f x %.1f)", 
        GetAttackPower(), baseAttackPower_, attackPowerMultiplier_);
    
    ImGui::Text("Defense: %.0f%% Damage Taken", 
        GetDefenseRate() * 100.0f);
    
    ImGui::Text("Move Speed: %.2f (Base: %.2f x %.2f)", 
        GetMoveSpeed(), baseMoveSpeed_, moveSpeedMultiplier_);
    
    // ▼▼▼ デバッグ用リセットボタン ▼▼▼
    if (ImGui::Button("Reset Status")) {
        baseAttackPower_ = 10.0f;
        attackPowerMultiplier_ = 1.0f;
        baseMaxHP_ = 100.0f;
        defenseMultiplier_ = 1.0f;
        baseMoveSpeed_ = 0.4f;
        moveSpeedMultiplier_ = 1.0f;
        HP = baseMaxHP_;
    }

    ImGui::End(); // 结束 UI 窗口
}
bool Player::IsOrbitBulletType(BulletType type) {
    return type == BulletType::Orbit ||
           type == BulletType::SpreadOrbit ||
           type == BulletType::AcceleratingOrbit||
           type == BulletType::TripleShotOrbit;
}
