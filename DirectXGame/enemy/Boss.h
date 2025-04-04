#pragma once
#include "../player/Player.h"
#include "Enemy.h"
#include "3d/Model.h"
#include "base/TextureManager.h"
#include "3d/WorldTransform.h"
#include "assert.h"
#include "math/Vector3.h"
#include "myMath.h"
#include <list>
#include <cmath>
#include <2d/ImGuiManager.h>



class Player;
// GameSceneクラスの前方宣言
class GameScene;
class Boss : public Enemy {
public:
    Boss();
    ~Boss();

    void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position) override; // 初始化
    void Update() override; // 更新逻辑
    void Draw(KamataEngine::Camera& camera) override;

    void TakeDamage(float damage); // Boss 受到伤害
    bool IsDead() const override; // 检查 Boss 是否死亡

	/*  void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }*/

     float GetBossRadius() const { return Bossradius_; }

     void BossDead() { isDead_ = true; }

private:
	Phase phase_ = Phase::Approach; // Boss 现在也有Phase状态


    float hp_ = 100; // Boss 的生命值
    KamataEngine::Vector3 circleCenter_ = { 0.0f, 0.0f, 0.0f }; // 场地中心
    float angle_ = 0.0f; // 绕场的角度


    // デスフラグ
    bool isDead_ = false;
	//Player* player_ = nullptr;
 //   GameScene* gameScene_ = nullptr; // 指向 GameScene 的指针
  
    float Bossradius_ = 1.5f; // Boss 的碰撞半径

     bool isSpawning_ = true;
    float spawnTimer_ = 0.0f;
    const float spawnDuration_ = 2.0f;
};