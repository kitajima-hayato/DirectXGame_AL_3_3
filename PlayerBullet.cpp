#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) { 
	//NULLポインタチェック
	assert(model); 
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	//引く数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() { 
	worldTransform_.UpadateMatrix();

	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
