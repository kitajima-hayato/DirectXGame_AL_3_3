#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <imgui.h>
#include <Input.h>
#include <MakeMatrix.h>
#include <list>
#include "PlayerBullet.h"
class Player {
public:
	~Player();
	void Initialize(Model*model,uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection&viewProjection);
	
	//旋回
	void Rotate();
	//弾の発射
	void Attack();

	//ワールド座標を取得
	Vector3 GetWorldPosition();


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
