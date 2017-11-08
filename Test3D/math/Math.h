#pragma once

#include <cmath>

namespace Math
{
static const float kSMALL_EPSILON = 0.00001f;
static const float kLARGE_EPSILON = 0.0001f;
static const float kHUGE_EPSILON  = 0.001f;
static const float kPI            = 3.14159265358979323846264338327950288f;
static const float kDEG2RAD       = kPI  / 180.0f;
static const float kDEG2RAD_2     = kPI  / 360.0f;    // kDEG2RAD / 2.f
static const float kRAD2DEG       = 1.0f / kDEG2RAD;

inline bool   Equals(float lhs, float rhs, float epsilon = kLARGE_EPSILON)
{
	return lhs + epsilon >= rhs && lhs - epsilon <= rhs;
}
inline bool   IsZero(float lhs, float epsilon = kLARGE_EPSILON) { return Equals(lhs, 0.f, epsilon); } 

inline float  Lerp(float lhs, float rhs, float t)   { return lhs * (1.0f - t) + rhs * t; }
inline double Lerp(double lhs, double rhs, float t) { return lhs * (1.0f - t) + rhs * t; }

inline float  Min(float lhs, float rhs)              { return lhs < rhs ? lhs : rhs; }
inline float  Max(float lhs, float rhs)              { return lhs > rhs ? lhs : rhs; }

inline float  Abs(float v)                           { return v >= 0.0f ? v : -v; }
inline float  Sign(float v)                          { return v > 0.0f ? 1.0f : (v < 0.0f ? -1.0f : 0.0f); }

inline bool   IsNaN(float value)
{
	unsigned u = *(unsigned*)(&value);
	return (u & 0x7fffffff) > 0x7f800000;
}

inline float  Clamp(float v, float min, float max)
{
	if (v < min)
		return min;
	else if (v > max)
		return max;
	else
		return v;
}

inline float  SmoothStep(float lhs, float rhs, float t)
{
	t = Clamp((t - lhs) / (rhs - lhs), 0.0f, 1.0f); // Saturate t
	return t * t * (3.0f - 2.0f * t);
}

inline int Min(int lhs, int rhs) { return lhs < rhs ? lhs : rhs; }
inline int Max(int lhs, int rhs) { return lhs > rhs ? lhs : rhs; }

inline int Abs(int value) { return value >= 0 ? value : -value; }

inline int Clamp(int value, int min, int max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

inline bool IsPowerOfTwo(unsigned v)
{
	if (!v)
		return true;
	while (!(v & 1))
		v >>= 1;
	return v == 1;
}

inline unsigned NextPowerOfTwo(unsigned v)
{
	unsigned ret = 1;
	while (ret < v && ret < 0x80000000)
		ret <<= 1;
	return ret;
}

inline unsigned CountSetBits(unsigned v)
{
	// Brian Kernighan's method
	unsigned count = 0;
	for (count = 0; v; count++)
		v &= v - 1;
	return count;
}

inline float SinDeg(float angle)         { return sinf(angle * kDEG2RAD); }
inline float CosDeg(float angle)         { return cosf(angle * kDEG2RAD); }
inline float TanDeg(float angle)         { return tanf(angle * kDEG2RAD); }
inline float AsinDeg(float x)            { return kRAD2DEG * asinf(Clamp(x, -1.0f, 1.0f)); }
inline float AcosDeg(float x)            { return kRAD2DEG * acosf(Clamp(x, -1.0f, 1.0f)); }
inline float AtanDeg(float x)            { return kRAD2DEG * atanf(x); }
inline float Atan2Deg(float y, float x)  { return kRAD2DEG * atan2f(y, x); }



}