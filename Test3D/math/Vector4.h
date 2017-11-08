#pragma once

#include "Vector3.h"
class Vector4
{
public:
	Vector4() :
		x(0.0f),
		y(0.0f),
		z(0.0f),
		w(0.0f)
	{
	}

	Vector4(const Vector4& vector) :
		x(vector.x),
		y(vector.y),
		z(vector.z),
		w(vector.w)
	{
	}

	Vector4(const Vector3& vector, float _w) :
		x(vector.x),
		y(vector.y),
		z(vector.z),
		w(_w)
	{
	}

	Vector4(float _x, float _y, float _z, float _w) :
		x(_x),
		y(_y),
		z(_z),
		w(_w)
	{
	}

	explicit Vector4(const float* data) :
		x(data[0]),
		y(data[1]),
		z(data[2]),
		w(data[3])
	{
	}

	Vector4& operator =(const Vector4& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	bool operator ==(const Vector4& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
	bool operator !=(const Vector4& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }

	Vector4 operator +(const Vector4& rhs) const { return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	Vector4 operator -() const { return Vector4(-x, -y, -z, -w); }
	Vector4 operator -(const Vector4& rhs) const { return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	Vector4 operator *(float rhs) const { return Vector4(x * rhs, y * rhs, z * rhs, w * rhs); }
	Vector4 operator *(const Vector4& rhs) const { return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
	Vector4 operator /(float rhs) const { return Vector4(x / rhs, y / rhs, z / rhs, w / rhs); }
	Vector4 operator /(const Vector4& rhs) const { return Vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }
	Vector4& operator +=(const Vector4& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}
	Vector4& operator -=(const Vector4& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}
	Vector4& operator *=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}
	Vector4& operator *=(const Vector4& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}
	Vector4& operator /=(float rhs)
	{
		float invRhs = 1.0f / rhs;
		x *= invRhs;
		y *= invRhs;
		z *= invRhs;
		w *= invRhs;
		return *this;
	}
	Vector4& operator /=(const Vector4& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}
	float Dot(const Vector4& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }

	float AbsDot(const Vector4& rhs) const
	{
		return Math::Abs(x * rhs.x) + Math::Abs(y * rhs.y) + Math::Abs(z * rhs.z) + Math::Abs(w * rhs.w);
	}

	Vector4 Abs() const { return Vector4(Math::Abs(x), Math::Abs(y), Math::Abs(z), Math::Abs(w)); }

	Vector4 Lerp(const Vector4& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

	bool Equals(const Vector4& rhs, float epsilon=Math::kLARGE_EPSILON) const
	{
		return Math::Equals(x, rhs.x, epsilon) && Math::Equals(y, rhs.y, epsilon) && 
			Math::Equals(z, rhs.z, epsilon) && Math::Equals(w, rhs.w, epsilon);
	}

	bool IsNaN() const { return Math::IsNaN(x) || Math::IsNaN(y) || Math::IsNaN(z) || Math::IsNaN(w); }

	const float* Data() const { return data; }

	union {
		float data[4];
		struct {
			float x, y, z, w;
		};
	};

	static const Vector4 ZERO;
	static const Vector4 ONE;
};

inline Vector4 operator *(float lhs, const Vector4& rhs) { return rhs * lhs; }