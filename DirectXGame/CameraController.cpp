#include "CameraController.h"
#include "Player.h"

void CameraController::Initialize(Camera* camera) { camera_ = camera; }

void CameraController::Update() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
Vector3 targetVelocity = target_->GetVelocity();
  Vector3 targetPosition = targetWorldTransform.translation_;
  Vector3 offset = {0.0f, 0.0f, -40.0f};
    Vector3 cameraPosition = targetPosition + offset;
	//===============================================================
	// 计算从相机位置到玩家位置的方向向量
  Vector3 direction = targetPosition - cameraPosition;

  // 使用 atan2 计算倾斜角度
  // atan2(y, z) 用于计算从下往上的倾斜角度
  float additionalTilt = 0.4f;
  float angleX = std::atan2(direction.y, direction.z)- additionalTilt; // 计算 X 轴的旋转角度
float desiredHeightAdjustment = std::abs(targetOffset_.z) * std::tan(angleX); 
 float fineTuneOffset = -5.0f;
  cameraPosition.y = targetPosition.y + targetOffset_.y + desiredHeightAdjustment+ fineTuneOffset; 
  // 设置相机的旋转角度
  camera_->rotation_.x = angleX; // 将计算的角度应用于相机
	camera_->translation_ = cameraPosition;
	//===============================================================
	// 補間追従
    camera_->translation_.x = std::lerp(camera_->translation_.x, cameraPosition.x, kInterpolationRate);
    camera_->translation_.y = std::lerp(camera_->translation_.y, cameraPosition.y, kInterpolationRate);
    camera_->translation_.z = std::lerp(camera_->translation_.z, cameraPosition.z, kInterpolationRate);

	//  追従対象画面外补正
	camera_->translation_.x = std::clamp(camera_->translation_.x, targetWorldTransform.translation_.x + margin.left, targetWorldTransform.translation_.x + margin.right);
	camera_->translation_.y = std::clamp(camera_->translation_.y, targetWorldTransform.translation_.y + margin.bottom, targetWorldTransform.translation_.y + margin.top);

	// 限制移动范围
	camera_->translation_.x = std::clamp(camera_->translation_.x, moveableArea_.left, moveableArea_.right);
	camera_->translation_.y = std::clamp(camera_->translation_.y, moveableArea_.bottom, moveableArea_.top);

	camera_->UpdateMatrix();
}

void CameraController::Reset() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	camera_->translation_ = Add(targetWorldTransform.translation_, targetOffset_);
}