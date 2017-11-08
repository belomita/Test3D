#include "Matrix3.h"
#include "Matrix4.h"
#include "Matrix3x4.h"


const Matrix3 Matrix3::ZERO(
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f);

const Matrix3 Matrix3::IDENTITY;

const Matrix4 Matrix4::ZERO(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f);

const Matrix4 Matrix4::IDENTITY;

const Matrix3x4 Matrix3x4::ZERO(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f);

const Matrix3x4 Matrix3x4::IDENTITY;

Matrix3 Matrix3::Inverse() const
{
	float det = m00 * m11 * m22 +
		m10 * m21 * m02 +
		m20 * m01 * m12 -
		m20 * m11 * m02 -
		m10 * m01 * m22 -
		m00 * m21 * m12;

	float invDet = 1.0f / det;

	return Matrix3(
		(m11 * m22 - m21 * m12) * invDet,
		-(m01 * m22 - m21 * m02) * invDet,
		(m01 * m12 - m11 * m02) * invDet,
		-(m10 * m22 - m20 * m12) * invDet,
		(m00 * m22 - m20 * m02) * invDet,
		-(m00 * m12 - m10 * m02) * invDet,
		(m10 * m21 - m20 * m11) * invDet,
		-(m00 * m21 - m20 * m01) * invDet,
		(m00 * m11 - m10 * m01) * invDet
		);
}

void Matrix4::Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const
{
	translation.x = m03;
	translation.y = m13;
	translation.z = m23;

	scale.x = sqrtf(m00 * m00 + m10 * m10 + m20 * m20);
	scale.y = sqrtf(m01 * m01 + m11 * m11 + m21 * m21);
	scale.z = sqrtf(m02 * m02 + m12 * m12 + m22 * m22);

	Vector3 invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
	rotation = Quaternion(ToMatrix3().Scaled(invScale));
}

Matrix4 Matrix4::Inverse() const
{
	float v0 = m20 * m31 - m21 * m30;
	float v1 = m20 * m32 - m22 * m30;
	float v2 = m20 * m33 - m23 * m30;
	float v3 = m21 * m32 - m22 * m31;
	float v4 = m21 * m33 - m23 * m31;
	float v5 = m22 * m33 - m23 * m32;

	float i00 = (v5 * m11 - v4 * m12 + v3 * m13);
	float i10 = -(v5 * m10 - v2 * m12 + v1 * m13);
	float i20 = (v4 * m10 - v2 * m11 + v0 * m13);
	float i30 = -(v3 * m10 - v1 * m11 + v0 * m12);

	float invDet = 1.0f / (i00 * m00 + i10 * m01 + i20 * m02 + i30 * m03);

	i00 *= invDet;
	i10 *= invDet;
	i20 *= invDet;
	i30 *= invDet;

	float i01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float i11 = (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float i21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float i31 = (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m10 * m31 - m11 * m30;
	v1 = m10 * m32 - m12 * m30;
	v2 = m10 * m33 - m13 * m30;
	v3 = m11 * m32 - m12 * m31;
	v4 = m11 * m33 - m13 * m31;
	v5 = m12 * m33 - m13 * m32;

	float i02 = (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float i12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float i22 = (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float i32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m21 * m10 - m20 * m11;
	v1 = m22 * m10 - m20 * m12;
	v2 = m23 * m10 - m20 * m13;
	v3 = m22 * m11 - m21 * m12;
	v4 = m23 * m11 - m21 * m13;
	v5 = m23 * m12 - m22 * m13;

	float i03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float i13 = (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float i23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float i33 = (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	return Matrix4(
		i00, i01, i02, i03,
		i10, i11, i12, i13,
		i20, i21, i22, i23,
		i30, i31, i32, i33);
}


void Matrix3x4::Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const
{
	translation.x = m03;
	translation.y = m13;
	translation.z = m23;

	scale.x = sqrtf(m00 * m00 + m10 * m10 + m20 * m20);
	scale.y = sqrtf(m01 * m01 + m11 * m11 + m21 * m21);
	scale.z = sqrtf(m02 * m02 + m12 * m12 + m22 * m22);

	Vector3 invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
	rotation = Quaternion(ToMatrix3().Scaled(invScale));
}

Matrix3x4 Matrix3x4::Inverse() const
{
	float det = m00 * m11 * m22 +
		m10 * m21 * m02 +
		m20 * m01 * m12 -
		m20 * m11 * m02 -
		m10 * m01 * m22 -
		m00 * m21 * m12;

	float invDet = 1.0f / det;
	Matrix3x4 ret;

	ret.m00 = (m11 * m22 - m21 * m12) * invDet;
	ret.m01 = -(m01 * m22 - m21 * m02) * invDet;
	ret.m02 = (m01 * m12 - m11 * m02) * invDet;
	ret.m03 = -(m03 * ret.m00 + m13 * ret.m01 + m23 * ret.m02);
	ret.m10 = -(m10 * m22 - m20 * m12) * invDet;
	ret.m11 = (m00 * m22 - m20 * m02) * invDet;
	ret.m12 = -(m00 * m12 - m10 * m02) * invDet;
	ret.m13 = -(m03 * ret.m10 + m13 * ret.m11 + m23 * ret.m12);
	ret.m20 = (m10 * m21 - m20 * m11) * invDet;
	ret.m21 = -(m00 * m21 - m20 * m01) * invDet;
	ret.m22 = (m00 * m11 - m10 * m01) * invDet;
	ret.m23 = -(m03 * ret.m20 + m13 * ret.m21 + m23 * ret.m22);

	return ret;
}