#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"
Enemy::~Enemy() {
	
}

void Enemy::Initialize(Model* model, const Vector3& position) {
	// NULLポインタのチェック
	assert(model);
	model_ = model;
	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("White.png");
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 操作入力
	input_ = Input::GetInstance();
	// 接近フェーズの初期化
	AccessInit();
}

void Enemy::Update() {
	// 移動速度
	std::cout << "Enemy updating..." << std::endl; // デバッグプリント
	switch (phase_) {
	case Phase::Approach: // 接近フェーズ
		Access();
		
		//Suicude();

		break;
	case Phase::Leave: // 離脱フェーズ
		Withdrawal();
		//Suicude();

		break;
	}
	#ifdef _DEBUG
	// キャラクターの座標を画面表示する処理
	/*ImGui::Begin("Enemy");
	ImGui::SliderFloat3("Position", &worldTransform_.translation_.x, -10.0f, 10.0f);
	ImGui::End();*/
	#endif
	// 行列更新
	worldTransform_.UpadateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾の描画
	/*for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}*/
}

void Enemy::AccessInit() {
	// 発射タイマーの初期化
	countDown = kFireInterval;
}

void Enemy::Access() {
	// 発射タイマーカウントダウン
	--countDown;
	// 指定時間に達したら
	if (countDown == 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		countDown = kFireInterval;
	}

	// 既定の位置に達したら離脱フェーズへ
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
	// 移動(ベクトルを加算)
	worldTransform_.translation_ -= velocity;
}

void Enemy::Withdrawal() { // 移動(ベクトルを加算)
	worldTransform_.translation_.z -= velocity.z;
}

void Enemy::Fire() {
	assert(player_);
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 playerPosition = player_->GetWorldPosition();
	Vector3 enemyPosition = GetWorldPosition();
	Vector3 distance = enemyPosition - playerPosition;
	distance = Normalize(distance);
	Vector3 velocityNormalize = {
	    distance.x * kBulletSpeed,
	    distance.y * kBulletSpeed,
	    distance.z * kBulletSpeed,
	}; 
	Vector3 velocityBullet = velocityNormalize;
	// 速度ベクトルをエネミー自身の向きに合わせて回転させる
	velocityBullet = TransformNormal(velocityBullet, worldTransform_.matWorld_);

	// 弾を生成し初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocityBullet);
	// 弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

//void Enemy::suicide() {
//	// デスフラグの立った弾の削除
//	bullets_.remove_if([](EnemyBullet* bullet) {
//		if (bullet->IsDead()) {
//			delete bullet;
//			return true;
//		}
//		return false;
//	});
//}

void Enemy::OnCollision() {



}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	worldPos = worldTransform_.translation_;
	return worldPos;
}

