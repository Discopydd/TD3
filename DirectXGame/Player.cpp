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
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();
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


	 Attack();
	for(PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
    // 碰撞检测
    CollisionMapInfo collisionMapInfo;
    collisionMapInfo.move = velocity_;
    MapCollision(collisionMapInfo);

    // 更新位置
    worldTransform_.translation_ += collisionMapInfo.move;

    // 更新变换矩阵
    worldTransform_.UpdateMatrix();
}


void Player::Draw()
{
   
    model_->Draw(worldTransform_, *camera_);
	for(PlayerBullet* bullet : bullets_) {
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
	if (input_->TriggerKey(DIK_SPACE))
	{
		const float kBulletSpeed = 1.0f;
        float bulletAngle = worldTransform_.rotation_.z;

        KamataEngine::Vector3 velocity(
            cos(bulletAngle) * kBulletSpeed, 
            sin(bulletAngle) * kBulletSpeed, 
            0);

        PlayerBullet* newBullet = new PlayerBullet();
        newBullet->Initialize(model_,GetWorldPosition(),velocity);

        bullets_.push_back(newBullet);
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