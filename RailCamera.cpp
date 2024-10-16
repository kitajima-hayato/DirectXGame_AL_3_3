#include "RailCamera.h"

void RailCamera::Initialize() {
	worldTransform_.Initialize();
	worldTransform_.translation_.z = -50;
	viewProjection_.farZ = 400.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	worldTransform_.translation_.z -= 0.5f;
	// worldTransform_.rotation_.z += 0.1f;

	worldTransform_.UpdateMatrixWorld();
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	
	ImGui::Begin("RailCamera");
	ImGui::SliderFloat3("RailCamera.translate", &worldTransform_.translation_.x, -100, 100);
	ImGui::SliderFloat3("RailCamera.rotate", &worldTransform_.rotation_.x, -1, 1);

	ImGui::End();
}


