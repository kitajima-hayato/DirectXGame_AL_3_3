#include "Enemy.h"

void Enemy::Initialize(Model* model, const Vector3& position) {
	//NULLポインタのチェック
	assert(model);
	model_ = model;
	//テクスチャの読み込み
	textureHandle_ = TextureManager::Load("Me.png");
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void Enemy::Update() {
	//移動速度
	const float kSpeed_ = 0.1f;
	Vector3 velocity = {0, 0, kSpeed_};
	worldTransform_.translation_ -= velocity;

	// 行列更新
	worldTransform_.UpadateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
