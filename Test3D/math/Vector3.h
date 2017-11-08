#pragma once

#include "math/Math.h"
#include "math/Vector2.h"

class Vector3
{
public:
	Vector3() : x(0.0f), y(0.0f), z(0.0f) { }
	Vector3(const Vector3& vector) : x(vector.x), y(vector.y), z(vector.z)   {}
	Vector3(const Vector2& vector, float z) : x(vector.x), y(vector.y), z(z) {}
	Vector3(const Vector2& vector) : x(vector.x), y(vector.y), z(0.0f)       {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)              {}
	Vector3(float _x, float _y) : x(_x), y(_y), z(0.0f)                      {}
	explicit Vector3(const float* data) : x(data[0]), y(data[1]), z(data[2]) {}

	Vector3& operator =(const Vector3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	bool operator ==(const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	bool operator !=(const Vector3& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - () const { return Vector3(-x, -y, -z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (float rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator * (const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
	Vector3 operator / (float rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	Vector3 operator / (const Vector3& rhs) const { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z); }
	Vector3& operator +=(const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vector3& operator -=(const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	Vector3& operator *=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	Vector3& operator *=(const Vector3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}
	Vector3& operator /=(float rhs)
	{
		float invRhs = 1.0f / rhs;
		x *= invRhs;
		y *= invRhs;
		z *= invRhs;
		return *this;
	}
	Vector3& operator /=(const Vector3& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}
	void Normalize()
	{
		float lenSquared = LengthSquared();
		if (!Math::Equals(lenSquared, 1.0f) && lenSquared > 0.0f) {
			float invLen = 1.0f / sqrtf(lenSquared);
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}
	}
	float Length() const        { return sqrtf(x * x + y * y + z * z); }
	float LengthSquared() const { return x * x + y * y + z * z; }
	float Dot(const Vector3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }
	float AbsDot(const Vector3& rhs) const
	{
		return Math::Abs(x * rhs.x) + Math::Abs(y * rhs.y) + Math::Abs(z * rhs.z);
	}
	Vector3 Cross(const Vector3& rhs) const
	{
		return Vector3(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x);
	}
	Vector3 Abs() const { return Vector3(Math::Abs(x), Math::Abs(y), Math::Abs(z)); }

	Vector3 Lerp(const Vector3& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

	bool Equals(const Vector3& rhs, float epsilon=Math::kLARGE_EPSILON) const
	{
		return Math::Equals(x, rhs.x, epsilon) && Math::Equals(y, rhs.y, epsilon) && Math::Equals(z, rhs.z, epsilon);
	}

	float Angle(const Vector3& rhs) const { return Math::AcosDeg(Dot(rhs) / (Length() * rhs.Length())); }

	bool IsNaN() const { return Math::IsNaN(x) || Math::IsNaN(y) || Math::IsNaN(z); }

	Vector3 Normalized() const
	{
		float lenSquared = LengthSquared();
		if (!Math::Equals(lenSquared, 1.0f) && lenSquared > 0.0f) {
			float invLen = 1.0f / sqrtf(lenSquared);
			return *this * invLen;
		}
		else
			return *this;
	}

	const float* Data() const { return data; }

	union {
		float data[3];
		struct {
			float x, y, z;
		};
		struct {
			float u, v, w;
		};
	};

	static const Vector3 ZERO;

	static const Vector3 LEFT;    /// (-1, 0, 0)
	static const Vector3 RIGHT;   /// ( 1, 0, 0)
	static const Vector3 UP;      /// ( 0, 0, 1)
	static const Vector3 DOWN;    /// ( 0, 0,-1)
	static const Vector3 FORWARD; /// ( 0, 1, 0)
	static const Vector3 BACK;    /// ( 0,-1, 0)
	static const Vector3 ONE;     /// ( 1, 1, 1)
};

inline Vector3 operator *(float lhs, const Vector3& rhs) { return rhs * lhs; }