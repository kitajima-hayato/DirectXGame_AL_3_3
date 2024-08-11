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
	// 移動速度
	
	
	switch (phase_) {
	case Phase::Approach:	//接近フェーズ
		Access();
		break;
	case Phase::Leave:		//離脱フェーズ
		Withdrawal();
		break;
	}
	// 行列更新
	worldTransform_.UpadateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Access() {
	// 移動(ベクトルを加算)
	worldTransform_.translation_ += velocity;
	// 既定の位置に達したら離脱フェーズへ
	if (worldTransform_.translation_.z > 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Withdrawal() {// 移動(ベクトルを加算)
	worldTransform_.translation_.y -= velocity.z;
}
