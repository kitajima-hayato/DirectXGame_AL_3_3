#pragma once
#include <iostream>

struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+(const Vector3& other) const { return Vector3{x + other.x, y + other.y, z + other.z}; }

	Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3 operator-(const Vector3& other) const { return Vector3{x - other.x, y - other.y, z - other.z}; }

	Vector3& operator-=(const Vector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector3 operator*(float scalar) const { return Vector3{x * scalar, y * scalar, z * scalar}; }

	Vector3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vector3 operator/(float scalar) const {
		if (scalar != 0) {
			return Vector3{x / scalar, y / scalar, z / scalar};
		} else {
			std::cerr << "Error: Division by zero" << std::endl;
			// ゼロで割る場合は何か適切な処理を行うか、デフォルトの Vector3 を返す
			return Vector3{0, 0, 0}; // 例: ゼロベクトルを返す
		}
	}

	Vector3& operator/=(float scalar) {
		if (scalar != 0) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
		} else {
			std::cerr << "Error: Division by zero" << std::endl;
		}
		return *this;
	}
};
