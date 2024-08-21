#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
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

	//衝突判定と応答
	void CheckALLCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	uint32_t textureHandle_ = 0;
	
	

	//デバッグカメラ有効
	bool isDebugCameraActiv_ = false;
	DebugCamera* debugCamera_ = nullptr;

	// 自機
	Player* player_ = nullptr;
	// 敵
	Enemy* enemy_ = nullptr;
	// 天球
	Skydome* skydome_ = nullptr;
	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// プレイヤー
	Model* model_ = nullptr;
	// エネミー
	Model* modelEnemy_ = nullptr;
	// 天球
	Model* modeldome_ = nullptr;

	//プレイヤーの座標をずらす
	float frontPos = -45.0f;
	
	
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
