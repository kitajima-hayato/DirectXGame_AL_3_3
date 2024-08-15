#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete modeldome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示のを有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が算書するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// エネミーの作成
	enemy_ = new Enemy();
	// エネミーの更新
	modelEnemy_ = Model::Create();
	Vector3 enemyPosition_ = {0, 2, 50.0f};
	enemy_->Initialize(modelEnemy_, enemyPosition_);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//スカイドーム
	skydome_ = new Skydome();
	modeldome_ = Model::CreateFromOBJ("skydome", true); 
	skydome_->Initialize(modeldome_, &viewProjection_);
	
} 

void GameScene::Update() {
	//天球の更新
	skydome_->Update();
	// 自キャラの更新
	player_->Update();
	// 敵の更新
	enemy_->Update();
	// デバッグカメラの更新
	debugCamera_->Update();
	// 全ての当たり判定
	CheckALLCollisions();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_1)) {
		isDebugCameraActiv_ = true;
	}
	// カメラの処理
	if (isDebugCameraActiv_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
#endif // _DEBUG
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	skydome_->Draw();
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる

	enemy_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckALLCollisions() {

	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		Vector3 distance = {
		    (posB.x - posA.x) * (posB.x - posA.x),
		    (posB.y - posA.y) * (posB.y - posA.y),
		    (posB.z - posA.z) * (posB.z - posA.z),
		};
		// 球と球の交差判定
		if (distance.x + distance.y + distance.z <= 0.3f) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
		if (distance.x + distance.y + distance.z <= -0.3f) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion
#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();
	// 自弾と敵キャラの当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 敵の座標
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		Vector3 distance = {
		    (posB.x - posA.x) * (posB.x - posA.x),
		    (posB.y - posA.y) * (posB.y - posA.y),
		    (posB.z - posA.z) * (posB.z - posA.z),
		};
		// 球と球の交差判定
		if (distance.x + distance.y + distance.z <= 0.8f) {
			// 敵の衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
		if (distance.x + distance.y + distance.z <= -0.8f) {
			// 敵の衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion
#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* bulletPlayer : playerBullets) {
		//自弾の座標
		posA = bulletPlayer->GetWorldPosition();
		//敵弾の座標
		for (EnemyBullet* bulletEnemy : enemyBullets) {
			posB = bulletEnemy->GetWorldPosition();
			//座標AとBの距離を求める
			Vector3 distance = {
			    (posB.x - posA.x) * (posB.x - posA.x),
			    (posB.y - posA.y) * (posB.y - posA.y),
			    (posB.z - posA.z) * (posB.z - posA.z),
			};
			// 球と球の交差判定
			if (distance.x + distance.y + distance.z <= 0.8f) {
				// 敵の衝突時コールバックを呼び出す
				bulletEnemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bulletPlayer->OnCollision();
			}
			if (distance.x + distance.y + distance.z <= -0.8f) {
				// 敵の衝突時コールバックを呼び出す
				bulletEnemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bulletPlayer->OnCollision();
			}
		}
	}
#pragma endregion
}
