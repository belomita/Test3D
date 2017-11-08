#pragma once
#include "Vector3.h"

class  Matrix3
{
public:

	Matrix3() :
		m00(1.0f),
		m01(0.0f),
		m02(0.0f),
		m10(0.0f),
		m11(1.0f),
		m12(0.0f),
		m20(0.0f),
		m21(0.0f),
		m22(1.0f)
	{
	}


	Matrix3(const Matrix3& matrix) :
		m00(matrix.m00),
		m01(matrix.m01),
		m02(matrix.m02),
		m10(matrix.m10),
		m11(matrix.m11),
		m12(matrix.m12),
		m20(matrix.m20),
		m21(matrix.m21),
		m22(matrix.m22)
	{
	}


	Matrix3(float v00, float v01, float v02,
		float v10, float v11, float v12,
		float v20, float v21, float v22) :
		m00(v00),
		m01(v01),
		m02(v02),
		m10(v10),
		m11(v11),
		m12(v12),
		m20(v20),
		m21(v21),
		m22(v22)
	{
	}


	explicit Matrix3(const float* data) :
		m00(data[0]),
		m01(data[1]),
		m02(data[2]),
		m10(data[3]),
		m11(data[4]),
		m12(data[5]),
		m20(data[6]),
		m21(data[7]),
		m22(data[8])
	{
	}


	Matrix3& operator =(const Matrix3& rhs)
	{
		m00 = rhs.m00;
		m01 = rhs.m01;
		m02 = rhs.m02;
		m10 = rhs.m10;
		m11 = rhs.m11;
		m12 = rhs.m12;
		m20 = rhs.m20;
		m21 = rhs.m21;
		m22 = rhs.m22;
		return *this;
	}


	bool operator ==(const Matrix3& rhs) const
	{
		const float* leftData = Data();
		const float* rightData = rhs.Data();

		for (unsigned i = 0; i < 9; ++i) {
			if (leftData[i] != rightData[i])
				return false;
		}

		return true;
	}


	bool operator !=(const Matrix3& rhs) const { return !(*this == rhs); }


	Vector3 operator *(const Vector3& rhs) const
	{
		return Vector3(
			m00 * rhs.x + m01 * rhs.y + m02 * rhs.z,
			m10 * rhs.x + m11 * rhs.y + m12 * rhs.z,
			m20 * rhs.x + m21 * rhs.y + m22 * rhs.z
			);
	}


	Matrix3 operator +(const Matrix3& rhs) const
	{
		return Matrix3(
			m00 + rhs.m00,
			m01 + rhs.m01,
			m02 + rhs.m02,
			m10 + rhs.m10,
			m11 + rhs.m11,
			m12 + rhs.m12,
			m20 + rhs.m20,
			m21 + rhs.m21,
			m22 + rhs.m22
			);
	}


	Matrix3 operator -(const Matrix3& rhs) const
	{
		return Matrix3(
			m00 - rhs.m00,
			m01 - rhs.m01,
			m02 - rhs.m02,
			m10 - rhs.m10,
			m11 - rhs.m11,
			m12 - rhs.m12,
			m20 - rhs.m20,
			m21 - rhs.m21,
			m22 - rhs.m22
			);
	}


	Matrix3 operator *(float rhs) const
	{
		return Matrix3(
			m00 * rhs,
			m01 * rhs,
			m02 * rhs,
			m10 * rhs,
			m11 * rhs,
			m12 * rhs,
			m20 * rhs,
			m21 * rhs,
			m22 * rhs
			);
	}


	Matrix3 operator *(const Matrix3& rhs) const
	{
		return Matrix3(
			m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20,
			m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21,
			m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,
			m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20,
			m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21,
			m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,
			m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20,
			m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21,
			m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22
			);
	}


	void SetScale(const Vector3& scale)
	{
		m00 = scale.x;
		m11 = scale.y;
		m22 = scale.z;
	}


	void SetScale(float scale)
	{
		m00 = scale;
		m11 = scale;
		m22 = scale;
	}


	Vector3 Scale() const
	{
		return Vector3(
			sqrtf(m00 * m00 + m10 * m10 + m20 * m20),
			sqrtf(m01 * m01 + m11 * m11 + m21 * m21),
			sqrtf(m02 * m02 + m12 * m12 + m22 * m22)
			);
	}


	Matrix3 Transpose() const
	{
		return Matrix3(
			m00,
			m10,
			m20,
			m01,
			m11,
			m21,
			m02,
			m12,
			m22
			);
	}


	Matrix3 Scaled(const Vector3& scale) const
	{
		return Matrix3(
			m00 * scale.x,
			m01 * scale.y,
			m02 * scale.z,
			m10 * scale.x,
			m11 * scale.y,
			m12 * scale.z,
			m20 * scale.x,
			m21 * scale.y,
			m22 * scale.z
			);
	}


	bool Equals(const Matrix3& rhs) const
	{
		const float* leftData = Data();
		const float* rightData = rhs.Data();

		for (unsigned i = 0; i < 9; ++i) {
			if (!Math::Equals(leftData[i], rightData[i]))
				return false;
		}

		return true;
	}


	Matrix3 Inverse() const;


	const float* Data() const { return data; }

	union {
		struct {
			float m00;
			float m01;
			float m02;
			float m10;
			float m11;
			float m12;
			float m20;
			float m21;
			float m22;
		};
		float data[9];
		float mat[3][3];
	};


	static void BulkTranspose(float* dest, const float* src, unsigned count)
	{
		for (unsigned i = 0; i < count; ++i) {
			dest[0] = src[0];
			dest[1] = src[3];
			dest[2] = src[6];
			dest[3] = src[1];
			dest[4] = src[4];
			dest[5] = src[7];
			dest[6] = src[2];
			dest[7] = src[5];
			dest[8] = src[8];

			dest += 9;
			src += 9;
		}
	}


	static const Matrix3 ZERO;

	static const Matrix3 IDENTITY;
};


inline Matrix3 operator *(float lhs, const Matrix3& rhs) { return rhs * lhs; }