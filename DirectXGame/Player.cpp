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

Player::~Player() {}

void Player::Initialize(Camera* camera, const Vector3& position)
{
    worldTransform_.Initialize();
    camera_ = camera;
    worldTransform_.translation_ = position;
    model_ = Model::CreateFromOBJ("Player", true);
}

void Player::Update() {
  

    #pragma region 1.移动输入


   if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
        Vector3 acceleration{};
        
        // 右キーが押された場合の処理
        if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
            // 現在の方向が右でない場合、方向と回転パラメータを更新する
            if (lrDirection_ != LRDirection::kRight) {
                lrDirection_ = LRDirection::kRight;
                turnStartRotationY_ = worldTransform_.rotation_.z;
                turnUseRotationY_ = (0.0f * PI) - worldTransform_.rotation_.z;
                turnNowFram_ = 1;
            }
            // 現在の速度が左向きの場合、速度を減衰させる
            if (velocity_.x < 0) {
                velocity_.x *= (1 - kAttenuation);
            }
            // 加速度を増加させる
            acceleration.x += kAcceleration;
        } else if (Input::GetInstance()->PushKey(DIK_LEFT)) { // 左キーが押された場合の処理
            // 現在の方向が左でない場合、方向と回転パラメータを更新する
            if (lrDirection_ != LRDirection::kLeft) {
                lrDirection_ = LRDirection::kLeft;
                turnStartRotationY_ = worldTransform_.rotation_.z;
                turnUseRotationY_ = (1.0f * PI) - worldTransform_.rotation_.z;
                turnNowFram_ = 1;
            }
            // 現在の速度が右向きの場合、速度を減衰させる
            if (velocity_.x > 0) {
                velocity_.x *= (1 - kAttenuation);
            }
            // 加速度を減少させる
            acceleration.x -= kAcceleration;
        }
        
        // 速度を更新する
        velocity_ += acceleration;
        // 速度範囲を制限する
        velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
    }else {
        // 左右キーが押されていない場合、速度を徐々に減衰させる
        velocity_.x *= (1 - kAttenuation);
    }
   if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_DOWN)) {
        Vector3 acceleration{};
        
        if (Input::GetInstance()->PushKey(DIK_UP)) {
            if (lrDirection_ != LRDirection::kUp) {
                lrDirection_ = LRDirection::kUp;
                turnStartRotationY_ = worldTransform_.rotation_.z;
                turnUseRotationY_ = (0.5f * PI) - worldTransform_.rotation_.z;
                turnNowFram_ = 1;
            }
            // 現在の速度が左向きの場合、速度を減衰させる
            if (velocity_.y > 0) {
                velocity_.y *= (1 - kAttenuation);
            }
            // 加速度を増加させる
            acceleration.y += kAcceleration;
        } else if (Input::GetInstance()->PushKey(DIK_DOWN)) { // 左キーが押された場合の処理
            // 現在の方向が左でない場合、方向と回転パラメータを更新する
            if (lrDirection_ != LRDirection::kDown) {
                lrDirection_ = LRDirection::kDown;
                turnStartRotationY_ = worldTransform_.rotation_.z;
                turnUseRotationY_ = (1.5f * PI) - worldTransform_.rotation_.z;
                turnNowFram_ = 1;
            }
            // 現在の速度が右向きの場合、速度を減衰させる
            if (velocity_.y < 0) {
                velocity_.y *= (1 - kAttenuation);
            }
            // 加速度を減少させる
            acceleration.y -= kAcceleration;
        }
        
        // 速度を更新する
        velocity_ += acceleration;
        // 速度範囲を制限する
        velocity_.y = std::clamp(velocity_.y, -kLimitRunSpeed, kLimitRunSpeed);
    }else {
        // 左右キーが押されていない場合、速度を徐々に減衰させる
        velocity_.y *= (1 - kAttenuation);
    }


    

   
    #pragma endregion

    // 2.考虑移动量进行碰撞检测
    CollisionMapInfo collisionMapInfo;
    collisionMapInfo.move = velocity_;
    MapCollision(collisionMapInfo);

    // 3.根据检测结果移动
    worldTransform_.translation_ += collisionMapInfo.move;

if (turnNowFram_ >= 1 && turnNowFram_ < turnEndFrame_) {
        turnNowFram_++;
        float easing = powf(float(turnNowFram_) / float(turnEndFrame_), 3);
        worldTransform_.rotation_.z = turnUseRotationY_ * easing + turnStartRotationY_;
    }
    worldTransform_.UpdateMatrix();

   
}

void Player::Draw()
{
   
    model_->Draw(worldTransform_, *camera_);
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