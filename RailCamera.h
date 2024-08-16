#pragma once
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <imgui.h>
class RailCamera {

public:
	void Initialize(Vector3 worldPosition, Vector3 rotation);
	void Update();

	ViewProjection& GetvieProjection(){ return viewProjection_; };

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};
