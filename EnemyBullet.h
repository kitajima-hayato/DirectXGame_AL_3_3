#pragma once
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Model.h>
#include <TextureManager.h>

class EnemyBullet {

public:
	void Initialize(Model*model,const Vector3 &position,const Vector3&velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool IsDead() const { return isDead_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//弾の速度
	Vector3 velocity_;
	//寿命-１秒６０フレームが５回つまり５フレーム
	static const int32_t kLifeTime = 60 * 5;
	//デス大麻
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
