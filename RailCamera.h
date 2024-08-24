#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <imgui.h>
class RailCamera {

public:
	void Initialize();
	void Update();
	// ワールドトランスフォームを取得
	const WorldTransform& GetWorldTrnasform() const { return worldTransform_; }

	const Matrix4x4& GetView() const { return viewProjection_.matView; }
	const Matrix4x4& GetViewProjection() const { return viewProjection_.matProjection; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;
};
