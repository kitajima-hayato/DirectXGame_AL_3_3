#include "Player.h"
#include <cassert>

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 initPos) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = initPos;
	input_ = Input::GetInstance();
}

void Player::Update() {
	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.5f;
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	// プレイヤーの旋回処理
	Rotate();
	// 移動限界座標
	const float kMoveLimitX = 30.0f;
	const float kMoveLimitY = 19.0f;

	// 範囲を超えない距離	/clampで下限上限設定
	worldTransform_.translation_.x = (std::max)(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = (std::min)(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = (std::max)(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = (std::min)(worldTransform_.translation_.y, +kMoveLimitY);

	// 座標移動
	worldTransform_.translation_ += move;
	Matrix4x4 affine = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = affine;

	

	// キャラクター攻撃処理
	Attack();
	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::SliderFloat3("Position", &worldTransform_.translation_.x, -10.0f, 10.0f);
	ImGui::End();

	// 定数バッファに転送する
	worldTransform_.UpadateMatrix();
}

void Player::Rotate() {
	// 回転の速さ[ラジアン/flame]
	const float kRotSpeed = 0.02f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	worldTransform_.UpadateMatrix();
}
void Player::Attack() {
	// 発射キー-トリガー-
	if (input_->TriggerKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// プレイヤーのワールド座標を取得
		Vector3 bulletPosition = GetWorldPosition();

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, bulletPosition, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

// void Player::Attack() {
//	// 発射キー-トリガー
//	if (input_->TriggerKey(DIK_SPACE)) {
//		// 弾の速度
//		const float kBukketSpeed = 1.0f;
//		Vector3 velocity(0, 0, kBukketSpeed);
//		// 速度ベクトルを自機の向きに合わせて回転させる
//		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
//		// 弾を生成し、初期化
//		PlayerBullet* newBullet = new PlayerBullet();
//		newBullet->Initialize(model_, GetWorldPosition(), velocity);
//		// 弾を登録する
//		bullets_.push_back(newBullet);
//	}
// }

void Player::OnCollision() {}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos = worldTransform_.translation_;

	// 親がいる場合、親のワールド行列を掛けてプレイヤーのワールド座標を求める
	if (worldTransform_.parent_) {
		Matrix4x4 parentMatrix = worldTransform_.parent_->matWorld_;
		worldPos = Transform(worldPos, parentMatrix);
	}

	return worldPos;
}
// Vector3 Player::GetWorldPosition() {
//	// ワールド座標を入れる変数
//	Vector3 worldPos;
//	// ワールド行列の平行移動成分を取得（ワールド座標）
//	worldPos.x = worldTransform_.translation_.x;
//	worldPos.y = worldTransform_.translation_.y;
//	worldPos.z = worldTransform_.translation_.z;
//	return worldPos;
// }

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
