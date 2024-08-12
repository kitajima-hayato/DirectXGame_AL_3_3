#pragma once
#include <Model.h>
#include <TextureManager.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <cassert>
#include <Input.h>
#include <imgui.h>
#include <list>
#include "EnemyBullet.h"
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

	//エネミーの接近フェーズの初期化関数
	void AccessInit();
	//エネミーの接近
	void Access();
	//エネミーの離脱
	void Withdrawal();
	//エネミーの攻撃
	void Fire();
	//弾の自爆機能ポインタ的に
	void Suicude();
	
	//発射間隔
	static const int kFireInterval = 60;


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	//エネミーの移動速度
	const float kSpeed_ = 0.1f;
	Vector3 velocity = {0, 0, kSpeed_};
	// フェーズ
	Phase phase_ = Phase::Approach;
	//弾
	std::list<EnemyBullet*> bullets_;
	// 操作入力仮　消すときはインクルードしたのも消すように
	Input* input_ = nullptr;

	//発射タイマー
	int32_t countDown = 0;
};
