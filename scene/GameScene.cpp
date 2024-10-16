#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	delete modeldome_;
	delete railCamera_;
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
	Vector3 pos = {0, 3.5f, frontPos};
	player_->Initialize(model_, textureHandle_, pos);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示のを有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が算書するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// エネミーの更新
	modelEnemy_ = Model::Create();
	// エネミーの作成

	// 天球
	skydome_ = new Skydome();
	modeldome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modeldome_, &viewProjection_);

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize();
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTrnasform());

	//csvの読み込み
	LoadEnemyPopData();
}

void GameScene::Update() {
	// レールカメラの更新
	railCamera_->Update();
	// 天球の更新
	skydome_->Update();
	// 自キャラの更新
	player_->Update();
	// 敵の更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
	UpdateEnemyPopCommands();
	// 敵の弾の更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
		AddEnemyBullet(bullet);
	}
	// 全ての当たり判定
	CheckALLCollisions();
	DethEnemy();
#pragma region viewProに値を渡す_レールカメラからゲームシーン
	viewProjection_.matView = railCamera_->GetView();
	viewProjection_.matProjection = railCamera_->GetViewProjection();
#pragma endregion
	viewProjection_.TransferMatrix();

	//	// デバッグカメラの更新
	//	debugCamera_->Update();
	// #ifdef _DEBUG
	//	if (input_->TriggerKey(DIK_1)) {
	//		isDebugCameraActiv_ = true;
	//	} else if (input_->TriggerKey(DIK_2)) {
	//		isDebugCameraActiv_ = false;
	//	}
	//	// カメラの処理
	//	if (isDebugCameraActiv_) {
	//		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
	//		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
	//		// ビュープロジェクション行列の転送
	//		viewProjection_.TransferMatrix();
	//	} else {
	//		// ビュープロジェクション行列の更新と転送
	//		viewProjection_.UpdateMatrix();
	//	}
	// #endif // _DEBUG
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
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
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
	// const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
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
	for (Enemy* enemy : enemys_) {
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();
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
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
			if (distance.x + distance.y + distance.z <= -0.8f) {
				// 敵の衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion
#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* bulletPlayer : playerBullets) {
		// 自弾の座標
		posA = bulletPlayer->GetWorldPosition();
		// 敵弾の座標
		for (EnemyBullet* bulletEnemy : enemyBullets_) {
			posB = bulletEnemy->GetWorldPosition();
			// 座標AとBの距離を求める
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::DethEnemy() {
	// デスフラグの立ったエネミーの削除
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
}

void GameScene::PopEnemy(Vector3 position) {
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(modelEnemy_, position);
	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	newEnemy->SetGameScene(this);
	enemys_.push_back(newEnemy);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv ");
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();
	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (stay) {
		stayTimer--;
		if (stayTimer <= 0) {
			// 待機完了
			stay = false;
		}
		return;
	}
	// 1行分の文字列を言えれる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {

		// 一行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		// 　,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント行
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			PopEnemy(Vector3(x,y,z));
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			waitTime = atoi(word.c_str());

			// 待機開始
			stay = true;          // 待機中フラグ
			stayTimer = waitTime; // 待機タイマー

			// コマンドループを抜ける
			break;
		}
	}
}