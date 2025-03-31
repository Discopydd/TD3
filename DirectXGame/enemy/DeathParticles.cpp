#include "DeathParticles.h"
#include <math/MathUtility.h>

DeathParticles::~DeathParticles()
{
	delete model_;
}

void DeathParticles::Initialize(KamataEngine::Camera* camera)
{
	model_ = KamataEngine::Model::CreateFromOBJ("deathParticle", true);
	camera_ = camera;
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.scale_ = { 0.5f, 1.0f, 0.5f };
	}
	objectColor_.Initialize();
	color_ = {1, 0, 0, 1};
}

void DeathParticles::Update()
{
	if (isFinished_|| !isStart) {
		return;
	}

  for (uint32_t i = 0; i < kNumParticles; ++i) {
        // 如果粒子未到地面，继续受重力影响
        if (worldTransforms_[i].translation_.z < groundHeight_) {
            velocity_[i] += gravity_; // 重力影响
            worldTransforms_[i].translation_ += velocity_[i]; // 更新位置
        } else {
            // 粒子落地后固定在地面
            worldTransforms_[i].translation_.z = groundHeight_;
            velocity_[i] = { velocity_[i].x * 0.9f, velocity_[i].y * 0.9f, 0.0f }; // 模拟血液落地后的小范围扩散
        }
    }
	//カウンターを1フレーム分の秒数進める
	
    //存続時間の上限に達したら
	if (counter_ <= kDuration) {
		counter_ += 1.0f / 60.0f;
	}else{
		//終了扱いにする
		isFinished_ = true;
	}
	for (auto& worldTransform : worldTransforms_) {

		worldTransform.UpdateMatrix();
	}

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	//色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
}

void DeathParticles::Draw()
{
	if (isFinished_|| !isStart) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *camera_, &objectColor_);
	}
}

void DeathParticles::SetStartPos(KamataEngine::Vector3 pos)
{
 for (uint32_t i = 0; i < kNumParticles; ++i) {
        worldTransforms_[i].translation_ = pos;

        // 随机喷溅方向
        float angle = (rand() % 360) * 3.14159265f / 180.0f;
        float speed = 0.1f + static_cast<float>(rand() % 10) * 0.01f; // 速度随机 0.1f ~ 0.2f
        velocity_[i] = {cos(angle) * speed, sin(angle) * speed, (rand() % 100) * 0.002f - 0.1f}; // 随机 Z 方向
    }
}

const bool DeathParticles::GetParticlesOver()
{
if (isFinished_)
		return true;
	return false;
}
