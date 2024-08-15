#pragma once
#include "Model.h"
#include "WorldTransform.h"
class Skydome {

public:
	void Initialize(Model*model,ViewProjection*viewprojection);
	void Update();
	void Draw();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_=nullptr;
	//モデルデータ
	Model* model_ = nullptr;
};
