#include "Skydome.h"
void Skydome::Initialize(Model* model, ViewProjection* viewprojection) {

	worldTransform_.Initialize();
	model_ = model;
	viewProjection_ = viewprojection;
}

void Skydome::Update() {}

void Skydome::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
