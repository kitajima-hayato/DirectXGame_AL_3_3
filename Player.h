#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <imgui.h>
#include <Input.h>
#include <MakeMatrix.h>
#include "PlayerBullet.h"
class Player {
public:
	void Initialize(Model*model,uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection&viewProjection);
	
	//旋回
	void Rotate();
	//弾の発射
	void Attack();

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
	PlayerBullet* bullet_ = nullptr;
};
