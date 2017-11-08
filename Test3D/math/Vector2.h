#pragma once

#include "math/Math.h"

class Vector2
{
public:
	Vector2() : x(0.0f), y(0.0f){ }
	Vector2(const Vector2& vector) : x(vector.x), y(vector.y) {}
	Vector2(float _x, float _y) : x(_x), y(_y) { }
	explicit Vector2(const float* data) : x(data[0]), y(data[1]) {}

	bool operator ==(const Vector2& rhs)   const { return x == rhs.x && y == rhs.y; }
	bool operator !=(const Vector2& rhs)   const { return x != rhs.x || y != rhs.y; }

	Vector2 operator +(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	Vector2 operator -() const                   { return Vector2(-x, -y); }
	Vector2 operator -(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
	Vector2 operator *(float rhs) const          { return Vector2(x * rhs, y * rhs); }
	Vector2 operator *(const Vector2& rhs) const { return Vector2(x * rhs.x, y * rhs.y); }
	Vector2 operator /(float rhs) const          { return Vector2(x / rhs, y / rhs); }
	Vector2 operator /(const Vector2& rhs) const { return Vector2(x / rhs.x, y / rhs.y); }

	Vector2& operator =(const Vector2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	Vector2& operator +=(const Vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vector2& operator -=(const Vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vector2& operator *=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	Vector2& operator *=(const Vector2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	Vector2& operator /=(float rhs)
	{
		float invRhs = 1.0f / rhs;
		x *= invRhs;
		y *= invRhs;
		return *this;
	}

	Vector2& operator /=(const Vector2& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	void Normalize()
	{
		float lenSquared = LengthSquared();
		if (!Math::Equals(lenSquared, 1.0f) && lenSquared > 0.0f) {
			float invLen = 1.0f / sqrtf(lenSquared);
			x *= invLen;
			y *= invLen;
		}
	}

	float Length() const                            { return sqrtf(x * x + y * y); }
	float LengthSquared() const                     { return x * x + y * y; }

	float Dot(const Vector2& rhs) const             { return x * rhs.x + y * rhs.y; }
	float AbsDot(const Vector2& rhs) const          { return Math::Abs(x * rhs.x) + Math::Abs(y * rhs.y); }

	Vector2 Abs() const                             { return Vector2(Math::Abs(x), Math::Abs(y)); }

	Vector2 Lerp(const Vector2& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

	bool Equals(const Vector2& rhs, float eplison=Math::kLARGE_EPSILON) const
	{ 
		return Math::Equals(x, rhs.x, eplison) && Math::Equals(y, rhs.y, eplison);
	}

	bool IsNaN() const                              { return Math::IsNaN(x) || Math::IsNaN(y); }

	Vector2 Normalized() const
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
		float data[2];
		struct {
			float x, y;
		};
		struct {
			float u, v;
		};
	};
	
	static const Vector2 ZERO;  // ( 0, 0)
	static const Vector2 LEFT;  // (-1, 0)
	static const Vector2 RIGHT; // ( 1, 0)
	static const Vector2 UP;    // ( 0, 1)
	static const Vector2 DOWN;  // ( 0,-1)
	static const Vector2 ONE;   // ( 1, 1)
};
inline Vector2 operator *(float lhs, const Vector2& rhs) { return rhs * lhs; }