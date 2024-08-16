#include "RailCamera.h"

void RailCamera::Initialize(Vector3 cameraPos,Vector3 cameraRotation) {
	
	// ワールドトランスフォームの初期化
	worldTransform_.translation_ = cameraPos;
	worldTransform_.rotation_ = cameraRotation;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}
 
void RailCamera::Update() {
	worldTransform_.translation_ += Vector3(0.0f, 0.0f, -1.0f);
	worldTransform_.UpadateMatrix();
	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	//カメラの座補油を画面表示する
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -10.0f, 10.0f); 
	ImGui::End();
}
