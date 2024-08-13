#pragma once
#include <Model.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <TextureManager.h>
#include <cassert>
class PlayerBullet {

public:
	//model-モデル//position-初期座標//velocity-速度
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	//関数名の後ろのconstは「このメンバ関数はメンバ変数を書き換えませんよ」という指定
	bool IsDead() const { return isDead_; }
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;
	//弾が消えないと大変なことになるので自滅してもらうためのセット
	//寿命-６０フレーム５回分＝５秒
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
