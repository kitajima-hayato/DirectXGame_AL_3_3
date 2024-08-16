#include "RailCamera.h"


void RailCamera::Initialize(WorldTransform* worldTransform) {
	worldTransform_.Initialize();
	worldTransform_.matWorld_ = worldTransform->matWorld_;
	worldTransform_.rotation_ = worldTransform->rotation_;
	worldTransform_.translation_.z = -50;
	//viewProjection_.farZ = 400.0f;
	viewProjection_.Initialize();
}
 
void RailCamera::Update() {
	//worldTransform_.rotation_ += Vector3(0.0f, 1.0f, 0.0f);
	worldTransform_.translation_ += Vector3(0.0f, 0.0f, 0.10f);
	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	// カメラの座標を画面表示する
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -100.0f, 100.0f);
	ImGui::End();
	worldTransform_.UpdateMatrixWorld();
	
}
