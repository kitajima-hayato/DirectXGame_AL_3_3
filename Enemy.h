#pragma once
#include <Model.h>
#include <TextureManager.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <cassert>
// 行動フェーズ
enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};
class Enemy {

public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	//エネミーの接近
	void Access();
	//エネミーの離脱
	void Withdrawal();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	//エネミーの移動速度
	const float kSpeed_ = 0.1f;
	Vector3 velocity = {0, 0, kSpeed_};
	// フェーズ
	Phase phase_ = Phase::Approach;
};
