#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <Input.h>
#include <MakeMatrix.h>
#include <list>
#include "PlayerBullet.h"
class Player {
public:
	~Player();
	void Initialize(Model*model,uint32_t textureHandle,Vector3 initPos);
	void Update();
	void Draw(ViewProjection&viewProjection);
	
	//旋回
	void Rotate();
	//弾の発射
	void Attack();
	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	
	//ワールド座標を取得
	Vector3 GetWorldPosition();
	
	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void SetParent(const WorldTransform* parent);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;
};
