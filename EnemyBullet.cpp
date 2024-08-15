#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("Red.png");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をセット
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	// 座標を移動させる(１フレーム分の移動量を足す)
	worldTransform_.translation_ -= velocity_;
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	// 行列更新
	worldTransform_.UpadateMatrix();
	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() {
	isDead_ = true; 
}

Vector3 EnemyBullet::GetWorldPosition() const {
	//ワールド座標を入れる変数
	Vector3 worldpos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldpos = worldTransform_.translation_;
	return worldpos;
}
