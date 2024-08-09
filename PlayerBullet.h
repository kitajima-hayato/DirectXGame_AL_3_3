#pragma once
#include <Model.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <TextureManager.h>
#include <cassert>
class PlayerBullet {

public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0u;

};
