#pragma once
#include "3d/Model.h"
#include "3d/WorldTransform.h"
#include "base/TextureManager.h"
#include "math/Vector3.h"
#include "assert.h"
#include "myMath.h"

#include <list>

class GameScene;

class Item {
public:
	Item();
	~Item();

	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position);
	void Update();
	void Draw(KamataEngine::Camera& camera);

	KamataEngine::Vector3 GetWorldPosition(); // 获取世界坐标
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsCollected() const { return isCollected_; } // 判断是否被拾取
	void Collect();                                   // 处理拾取逻辑
	 bool IsFullyCollected() const;                   //判断是否完全消失
private:
	GameScene* gameScene_ = nullptr;              // 关联游戏场景
	KamataEngine::WorldTransform worldTransform_; // 变换数据
	KamataEngine::Model* model_ = nullptr;        // 模型
	bool isCollected_ = false;                    // 是否被拾取的标志
	 bool isFullyCollected_ = false; // 新增：完全消失的标志
    float moveTimer_ = 0.0f;        // 新增：拾取后的移动计时器
};
