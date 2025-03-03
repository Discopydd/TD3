#pragma once
#include <cmath>

namespace KamataEngine {

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	inline Vector3 operator*(float scalar) const {
    return { x * scalar, y * scalar, z * scalar };
}

};

//运算符重载
inline Vector3 operator+(const Vector3& leftVector, const Vector3& rightVector) { return Vector3(leftVector.x + rightVector.x, leftVector.y + rightVector.y, leftVector.z + rightVector.z); }

inline Vector3 operator-(const Vector3& leftVector, const Vector3& rightVector) { return Vector3(leftVector.x - rightVector.x, leftVector.y - rightVector.y, leftVector.z - rightVector.z); }

inline Vector3& operator+=(Vector3& leftVector, const Vector3& rightVector) {
	leftVector.x += rightVector.x;
	leftVector.y += rightVector.y;
	leftVector.z += rightVector.z;
	return leftVector;
}

inline Vector3& operator-=(Vector3& leftVector, const Vector3& rightVector) {
	leftVector.x -= rightVector.x;
	leftVector.y -= rightVector.y;
	leftVector.z -= rightVector.z;
	return leftVector;
}

inline Vector3 operator*(float Scaler, const Vector3& v) { return Vector3(Scaler * v.x, Scaler * v.y, Scaler * v.z); }
inline Vector3 Add(const Vector3& a, const Vector3& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}
inline Vector3 Normalize(const Vector3& vec) {
   float length = static_cast<float>(sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));

    if (length > 0) {
        return { vec.x / length, vec.y / length, vec.z / length };
    }
    return vec;
}

inline float Length(const Vector3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
} // namespace KamataEngine
