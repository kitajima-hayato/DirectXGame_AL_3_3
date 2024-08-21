#pragma once
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <imgui.h>
class RailCamera {
public:
	void Initialize(WorldTransform& worldTransform);
	void Update();

	const Matrix4x4& GetViewProjectionMatview() const { return viewProjection_.matView; };
	const Matrix4x4& GetviewProjectionMatPro() const { return viewProjection_.matProjection; };
	//
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};
