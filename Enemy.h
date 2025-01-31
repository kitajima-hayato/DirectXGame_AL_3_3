#pragma once
#include <Model.h>
#include <TextureManager.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <cassert>
#include <Input.h>
#include <list>
#include "EnemyBullet.h"
//自機クラスの前方宣言
class Player;
//ゲームシーンの前方宣言
class GameScene;
// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};
class Enemy {

public:
	~Enemy();
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void SetPlayer(Player* player) { player_ = player; }

	// エネミーの接近フェーズの初期化関数
	void AccessInit();
	// エネミーの接近
	void Access();
	// エネミーの離脱
	void Withdrawal();
	// エネミーの攻撃
	void Fire();
	// 弾の自爆機能ポインタ的に
	//void suicide();
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDeadEnemy_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 発射間隔
	static const int kFireInterval = 60;
	// 弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	// エネミーの移動速度
	const float kSpeed_ = 0.05f;
	Vector3 velocity = {0, 0, kSpeed_};
	// フェーズ
	Phase phase_ = Phase::Approach;
	// 弾
	// 操作入力仮　消すときはインクルードしたのも消すように
	Input* input_ = nullptr;
	// 発射タイマー
	int32_t countDown = 0;
	// 自キャラ
	Player* player_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// エネミーのデスフラグ
	bool isDeadEnemy_ = false;
};
