#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "RailCamera.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include <sstream>
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 衝突判定と応答
	void CheckALLCollisions();

	// 弾の生成
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	//

	// 敵のデスフラグがたったらdelete
	void DethEnemy();
	//敵の発生関数
	void PopEnemy(Vector3 position);

	// 敵発生データの読み込み
	void LoadEnemyPopData();
	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;

	Model* model_ = nullptr;
	ViewProjection viewProjection_;
	// 自機
	Player* player_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActiv_ = false;
	DebugCamera* debugCamera_ = nullptr;
	// 敵
	std::list<Enemy*> enemys_;
	Model* modelEnemy_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;
	Model* modeldome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	WorldTransform worldTransform_;

	// プレイヤーの座標を前にずらす
	float frontPos = 10.0f;

	// 弾
	std::list<EnemyBullet*> enemyBullets_;

	//敵の発生制御
	int32_t waitTime;
	bool stay = true;             // 待機中フラグ
	int32_t stayTimer = waitTime; // 待機タイマー

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
