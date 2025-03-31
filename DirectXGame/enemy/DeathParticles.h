#pragma once
#include <3d/Model.h>
#include <3d/Camera.h>
#include <3d/WorldTransform.h>
#include <algorithm>
#include <array>
#include <cmath>
///
///
/// 
class  DeathParticles
{
private:
	static inline const uint32_t kNumParticles = 20; // 生成更多血滴
	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Vector3 velocity_[kNumParticles];  // 速度
 KamataEngine::Vector3 gravity_ = {0.0f, 0.0f, 0.02f};  // 血滴受重力影响

	
	static inline const float kDuration = 1.5f;                              // 存続時間<秒>
	static inline const float kSpeed = 0.1f;                              // スピード
	static inline const float kAngleUnit = acosf(-1) * 2 / kNumParticles; // 分割下1個分の角度

	bool isFinished_= false;
	float counter_ = 0;
	bool isStart = false;

	KamataEngine::ObjectColor objectColor_; // 色変更オブジェクト
	KamataEngine::Vector4 color_;           // 色
	float groundHeight_ = 1.0f;
public:
	~DeathParticles();
	void Initialize(KamataEngine::Camera* camera_);
	void Update();
	void Draw();


	void SetIsStart(bool flag) { isStart = flag;  };
	void SetStartPos(KamataEngine::Vector3 pos);
	const bool GetParticlesOver();
};
