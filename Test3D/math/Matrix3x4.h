#pragma once

#include "Matrix4.h"

#ifdef MATH_SSE
#include <emmintrin.h>
#endif

/// 3x4 matrix for scene node transform calculations.
class Matrix3x4
{
public:
    /// Construct an identity matrix.
    Matrix3x4()
#ifndef MATH_SSE
       :m00(1.0f),
        m01(0.0f),
        m02(0.0f),
        m03(0.0f),
        m10(0.0f),
        m11(1.0f),
        m12(0.0f),
        m13(0.0f),
        m20(0.0f),
        m21(0.0f),
        m22(1.0f),
        m23(0.0f)
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&m00, _mm_set_ps(0.f, 0.f, 0.f, 1.f));
        _mm_storeu_ps(&m10, _mm_set_ps(0.f, 0.f, 1.f, 0.f));
        _mm_storeu_ps(&m20, _mm_set_ps(0.f, 1.f, 0.f, 0.f));
#endif
    }

    /// Copy-construct from another matrix.
    Matrix3x4(const Matrix3x4& matrix)
#ifndef MATH_SSE
       :m00(matrix.m00),
        m01(matrix.m01),
        m02(matrix.m02),
        m03(matrix.m03),
        m10(matrix.m10),
        m11(matrix.m11),
        m12(matrix.m12),
        m13(matrix.m13),
        m20(matrix.m20),
        m21(matrix.m21),
        m22(matrix.m22),
        m23(matrix.m23)
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&m00, _mm_loadu_ps(&matrix.m00));
        _mm_storeu_ps(&m10, _mm_loadu_ps(&matrix.m10));
        _mm_storeu_ps(&m20, _mm_loadu_ps(&matrix.m20));
#endif
    }

    /// Copy-construct from a 3x3 matrix and set the extra elements to identity.
    Matrix3x4(const Matrix3& matrix) :
        m00(matrix.m00),
        m01(matrix.m01),
        m02(matrix.m02),
        m03(0.0f),
        m10(matrix.m10),
        m11(matrix.m11),
        m12(matrix.m12),
        m13(0.0f),
        m20(matrix.m20),
        m21(matrix.m21),
        m22(matrix.m22),
        m23(0.0f)
    {
    }

    /// Copy-construct from a 4x4 matrix which is assumed to contain no projection.
    Matrix3x4(const Matrix4& matrix)
#ifndef MATH_SSE
       :m00(matrix.m00),
        m01(matrix.m01),
        m02(matrix.m02),
        m03(matrix.m03),
        m10(matrix.m10),
        m11(matrix.m11),
        m12(matrix.m12),
        m13(matrix.m13),
        m20(matrix.m20),
        m21(matrix.m21),
        m22(matrix.m22),
        m23(matrix.m23)
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&m00, _mm_loadu_ps(&matrix.m00));
        _mm_storeu_ps(&m10, _mm_loadu_ps(&matrix.m10));
        _mm_storeu_ps(&m20, _mm_loadu_ps(&matrix.m20));
#endif
    }

    // Construct from values.
    Matrix3x4(float v00, float v01, float v02, float v03,
              float v10, float v11, float v12, float v13,
              float v20, float v21, float v22, float v23) :
        m00(v00),
        m01(v01),
        m02(v02),
        m03(v03),
        m10(v10),
        m11(v11),
        m12(v12),
        m13(v13),
        m20(v20),
        m21(v21),
        m22(v22),
        m23(v23)
    {
    }

    /// Construct from a float array.
    explicit Matrix3x4(const float* data)
#ifndef MATH_SSE
       :m00(data[0]),
        m01(data[1]),
        m02(data[2]),
        m03(data[3]),
        m10(data[4]),
        m11(data[5]),
        m12(data[6]),
        m13(data[7]),
        m20(data[8]),
        m21(data[9]),
        m22(data[10]),
        m23(data[11])
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&m00, _mm_loadu_ps(data));
        _mm_storeu_ps(&m10, _mm_loadu_ps(data + 4));
        _mm_storeu_ps(&m20, _mm_loadu_ps(data + 8));
#endif
    }

    /// Construct from translation, rotation and uniform scale.
    Matrix3x4(const Vector3& translation, const Quaternion& rotation, float scale)
    {
#ifdef MATH_SSE
        __m128 t = _mm_set_ps(1.f, translation.z, translation.y, translation.x);
        __m128 q = _mm_loadu_ps(&rotation.w);
        __m128 s = _mm_set_ps(1.f, scale, scale, scale);
        SetFromTRS(t, q, s);
#else
        SetRotation(rotation.RotationMatrix() * scale);
        SetTranslation(translation);
#endif
    }

    /// Construct from translation, rotation and nonuniform scale.
    Matrix3x4(const Vector3& translation, const Quaternion& rotation, const Vector3& scale)
    {
#ifdef MATH_SSE
        __m128 t = _mm_set_ps(1.f, translation.z, translation.y, translation.x);
        __m128 q = _mm_loadu_ps(&rotation.w);
        __m128 s = _mm_set_ps(1.f, scale.z, scale.y, scale.x);
        SetFromTRS(t, q, s);
#else
        SetRotation(rotation.RotationMatrix().Scaled(scale));
        SetTranslation(translation);
#endif
    }

    /// Assign from another matrix.
    Matrix3x4& operator =(const Matrix3x4& rhs)
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&m00, _mm_loadu_ps(&rhs.m00));
        _mm_storeu_ps(&m10, _mm_loadu_ps(&rhs.m10));
        _mm_storeu_ps(&m20, _mm_loadu_ps(&rhs.m20));
#else
        m00 = rhs.m00;
        m01 = rhs.m01;
        m02 = rhs.m02;
        m03 = rhs.m03;
        m10 = rhs.m10;
        m11 = rhs.m11;
        m12 = rhs.m12;
        m13 = rhs.m13;
        m20 = rhs.m20;
        m21 = rhs.m21;
        m22 = rhs.m22;
        m23 = rhs.m23;
#endif
        return *this;
    }

    /// Assign from a 3x3 matrix and set the extra elements to identity.
    Matrix3x4& operator =(const Matrix3& rhs)
    {
        m00 = rhs.m00;
        m01 = rhs.m01;
        m02 = rhs.m02;
        m03 = 0.0;
        m10 = rhs.m10;
        m11 = rhs.m11;
        m12 = rhs.m12;
        m13 = 0.0;
        m20 = rhs.m20;
        m21 = rhs.m21;
        m22 = rhs.m22;
        m23 = 0.0;
        return *this;
    }

    /// Assign from a 4x4 matrix which is assumed to contain no projection.
    Matrix3x4& operator =(const Matrix4& rhs)
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&m00, _mm_loadu_ps(&rhs.m00));
        _mm_storeu_ps(&m10, _mm_loadu_ps(&rhs.m10));
        _mm_storeu_ps(&m20, _mm_loadu_ps(&rhs.m20));
#else
        m00 = rhs.m00;
        m01 = rhs.m01;
        m02 = rhs.m02;
        m03 = rhs.m03;
        m10 = rhs.m10;
        m11 = rhs.m11;
        m12 = rhs.m12;
        m13 = rhs.m13;
        m20 = rhs.m20;
        m21 = rhs.m21;
        m22 = rhs.m22;
        m23 = rhs.m23;
#endif
        return *this;
    }

    /// Test for equality with another matrix without epsilon.
    bool operator ==(const Matrix3x4& rhs) const
    {
#ifdef MATH_SSE
        __m128 c0 = _mm_cmpeq_ps(_mm_loadu_ps(&m00), _mm_loadu_ps(&rhs.m00));
        __m128 c1 = _mm_cmpeq_ps(_mm_loadu_ps(&m10), _mm_loadu_ps(&rhs.m10));
        c0 = _mm_and_ps(c0, c1);
        __m128 c2 = _mm_cmpeq_ps(_mm_loadu_ps(&m20), _mm_loadu_ps(&rhs.m20));
        c0 = _mm_and_ps(c0, c2);
        __m128 hi = _mm_movehl_ps(c0, c0);
        c0 = _mm_and_ps(c0, hi);
        hi = _mm_shuffle_ps(c0, c0, _MM_SHUFFLE(1, 1, 1, 1));
        c0 = _mm_and_ps(c0, hi);
        return !_mm_ucomige_ss(c0, c0);
#else
        const float* leftData = Data();
        const float* rightData = rhs.Data();

        for (unsigned i = 0; i < 12; ++i)
        {
            if (leftData[i] != rightData[i])
                return false;
        }

        return true;
#endif
    }

    /// Test for inequality with another matrix without epsilon.
    bool operator !=(const Matrix3x4& rhs) const { return !(*this == rhs); }

    /// Multiply a Vector3 which is assumed to represent position.
    Vector3 operator *(const Vector3& rhs) const
    {
#ifdef MATH_SSE
        __m128 vec = _mm_set_ps(1.f, rhs.z, rhs.y, rhs.x);
        __m128 r0 = _mm_mul_ps(_mm_loadu_ps(&m00), vec);
        __m128 r1 = _mm_mul_ps(_mm_loadu_ps(&m10), vec);
        __m128 t0 = _mm_unpacklo_ps(r0, r1);
        __m128 t1 = _mm_unpackhi_ps(r0, r1);
        t0 = _mm_add_ps(t0, t1);
        __m128 r2 = _mm_mul_ps(_mm_loadu_ps(&m20), vec);
        __m128 r3 = _mm_setzero_ps();
        __m128 t2 = _mm_unpacklo_ps(r2, r3);
        __m128 t3 = _mm_unpackhi_ps(r2, r3);
        t2 = _mm_add_ps(t2, t3);
        vec = _mm_add_ps(_mm_movelh_ps(t0, t2), _mm_movehl_ps(t2, t0));

        return Vector3(
            _mm_cvtss_f32(vec),
            _mm_cvtss_f32(_mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 1, 1, 1))),
            _mm_cvtss_f32(_mm_movehl_ps(vec, vec)));
#else
        return Vector3(
            (m00 * rhs.x + m01 * rhs.y + m02 * rhs.z + m03),
            (m10 * rhs.x + m11 * rhs.y + m12 * rhs.z + m13),
            (m20 * rhs.x + m21 * rhs.y + m22 * rhs.z + m23)
        );
#endif
    }

    /// Multiply a Vector4.
    Vector3 operator *(const Vector4& rhs) const
    {
#ifdef MATH_SSE
        __m128 vec = _mm_loadu_ps(&rhs.x);
        __m128 r0 = _mm_mul_ps(_mm_loadu_ps(&m00), vec);
        __m128 r1 = _mm_mul_ps(_mm_loadu_ps(&m10), vec);
        __m128 t0 = _mm_unpacklo_ps(r0, r1);
        __m128 t1 = _mm_unpackhi_ps(r0, r1);
        t0 = _mm_add_ps(t0, t1);
        __m128 r2 = _mm_mul_ps(_mm_loadu_ps(&m20), vec);
        __m128 r3 = _mm_setzero_ps();
        __m128 t2 = _mm_unpacklo_ps(r2, r3);
        __m128 t3 = _mm_unpackhi_ps(r2, r3);
        t2 = _mm_add_ps(t2, t3);
        vec = _mm_add_ps(_mm_movelh_ps(t0, t2), _mm_movehl_ps(t2, t0));

        return Vector3(
            _mm_cvtss_f32(vec),
            _mm_cvtss_f32(_mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 1, 1, 1))),
            _mm_cvtss_f32(_mm_movehl_ps(vec, vec)));
#else
        return Vector3(
            (m00 * rhs.x + m01 * rhs.y + m02 * rhs.z + m03 * rhs.w),
            (m10 * rhs.x + m11 * rhs.y + m12 * rhs.z + m13 * rhs.w),
            (m20 * rhs.x + m21 * rhs.y + m22 * rhs.z + m23 * rhs.w)
        );
#endif
    }

    /// Add a matrix.
    Matrix3x4 operator +(const Matrix3x4& rhs) const
    {
#ifdef MATH_SSE
        Matrix3x4 ret;
        _mm_storeu_ps(&ret.m00, _mm_add_ps(_mm_loadu_ps(&m00), _mm_loadu_ps(&rhs.m00)));
        _mm_storeu_ps(&ret.m10, _mm_add_ps(_mm_loadu_ps(&m10), _mm_loadu_ps(&rhs.m10)));
        _mm_storeu_ps(&ret.m20, _mm_add_ps(_mm_loadu_ps(&m20), _mm_loadu_ps(&rhs.m20)));
        return ret;
#else
        return Matrix3x4(
            m00 + rhs.m00,
            m01 + rhs.m01,
            m02 + rhs.m02,
            m03 + rhs.m03,
            m10 + rhs.m10,
            m11 + rhs.m11,
            m12 + rhs.m12,
            m13 + rhs.m13,
            m20 + rhs.m20,
            m21 + rhs.m21,
            m22 + rhs.m22,
            m23 + rhs.m23
        );
#endif
    }

    /// Subtract a matrix.
    Matrix3x4 operator -(const Matrix3x4& rhs) const
    {
#ifdef MATH_SSE
        Matrix3x4 ret;
        _mm_storeu_ps(&ret.m00, _mm_sub_ps(_mm_loadu_ps(&m00), _mm_loadu_ps(&rhs.m00)));
        _mm_storeu_ps(&ret.m10, _mm_sub_ps(_mm_loadu_ps(&m10), _mm_loadu_ps(&rhs.m10)));
        _mm_storeu_ps(&ret.m20, _mm_sub_ps(_mm_loadu_ps(&m20), _mm_loadu_ps(&rhs.m20)));
        return ret;
#else
        return Matrix3x4(
            m00 - rhs.m00,
            m01 - rhs.m01,
            m02 - rhs.m02,
            m03 - rhs.m03,
            m10 - rhs.m10,
            m11 - rhs.m11,
            m12 - rhs.m12,
            m13 - rhs.m13,
            m20 - rhs.m20,
            m21 - rhs.m21,
            m22 - rhs.m22,
            m23 - rhs.m23
        );
#endif
    }

    /// Multiply with a scalar.
    Matrix3x4 operator *(float rhs) const
    {
#ifdef MATH_SSE
        Matrix3x4 ret;
        const __m128 mul = _mm_set1_ps(rhs);
        _mm_storeu_ps(&ret.m00, _mm_mul_ps(_mm_loadu_ps(&m00), mul));
        _mm_storeu_ps(&ret.m10, _mm_mul_ps(_mm_loadu_ps(&m10), mul));
        _mm_storeu_ps(&ret.m20, _mm_mul_ps(_mm_loadu_ps(&m20), mul));
        return ret;
#else
        return Matrix3x4(
            m00 * rhs,
            m01 * rhs,
            m02 * rhs,
            m03 * rhs,
            m10 * rhs,
            m11 * rhs,
            m12 * rhs,
            m13 * rhs,
            m20 * rhs,
            m21 * rhs,
            m22 * rhs,
            m23 * rhs
        );
#endif
    }

    /// Multiply a matrix.
    Matrix3x4 operator *(const Matrix3x4& rhs) const
    {
#ifdef MATH_SSE
        Matrix3x4 out;

        __m128 r0 = _mm_loadu_ps(&rhs.m00);
        __m128 r1 = _mm_loadu_ps(&rhs.m10);
        __m128 r2 = _mm_loadu_ps(&rhs.m20);
        __m128 r3 = _mm_set_ps(1.f, 0.f, 0.f, 0.f);

        __m128 l = _mm_loadu_ps(&m00);
        __m128 t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
        __m128 t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
        __m128 t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
        __m128 t3 = _mm_mul_ps(l, r3);
        _mm_storeu_ps(&out.m00, _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));

        l = _mm_loadu_ps(&m10);
        t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
        t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
        t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
        t3 = _mm_mul_ps(l, r3);
        _mm_storeu_ps(&out.m10, _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));

        l = _mm_loadu_ps(&m20);
        t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
        t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
        t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
        t3 = _mm_mul_ps(l, r3);
        _mm_storeu_ps(&out.m20, _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));

        return out;
#else
        return Matrix3x4(
            m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20,
            m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21,
            m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,
            m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03,
            m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20,
            m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21,
            m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,
            m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13,
            m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20,
            m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21,
            m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22,
            m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23
        );
#endif
    }

    /// Multiply a 4x4 matrix.
    Matrix4 operator *(const Matrix4& rhs) const
    {
#ifdef MATH_SSE
        Matrix4 out;

        __m128 r0 = _mm_loadu_ps(&rhs.m00);
        __m128 r1 = _mm_loadu_ps(&rhs.m10);
        __m128 r2 = _mm_loadu_ps(&rhs.m20);
        __m128 r3 = _mm_loadu_ps(&rhs.m30);

        __m128 l = _mm_loadu_ps(&m00);
        __m128 t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
        __m128 t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
        __m128 t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
        __m128 t3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
        _mm_storeu_ps(&out.m00, _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));

        l = _mm_loadu_ps(&m10);
        t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
        t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
        t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
        t3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
        _mm_storeu_ps(&out.m10, _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));

        l = _mm_loadu_ps(&m20);
        t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
        t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
        t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
        t3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
        _mm_storeu_ps(&out.m20, _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));

        _mm_storeu_ps(&out.m30, r3);

        return out;
#else
        return Matrix4(
            m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30,
            m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31,
            m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32,
            m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33,
            m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30,
            m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31,
            m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32,
            m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33,
            m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30,
            m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31,
            m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32,
            m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33,
            rhs.m30,
            rhs.m31,
            rhs.m32,
            rhs.m33
        );
#endif
    }

    /// Set translation elements.
    void SetTranslation(const Vector3& translation)
    {
        m03 = translation.x;
        m13 = translation.y;
        m23 = translation.z;
    }

    /// Set rotation elements from a 3x3 matrix.
    void SetRotation(const Matrix3& rotation)
    {
        m00 = rotation.m00;
        m01 = rotation.m01;
        m02 = rotation.m02;
        m10 = rotation.m10;
        m11 = rotation.m11;
        m12 = rotation.m12;
        m20 = rotation.m20;
        m21 = rotation.m21;
        m22 = rotation.m22;
    }

    /// Set scaling elements.
    void SetScale(const Vector3& scale)
    {
        m00 = scale.x;
        m11 = scale.y;
        m22 = scale.z;
    }

    /// Set uniform scaling elements.
    void SetScale(float scale)
    {
        m00 = scale;
        m11 = scale;
        m22 = scale;
    }

    /// Return the combined rotation and scaling matrix.
    Matrix3 ToMatrix3() const
    {
        return Matrix3(
            m00,
            m01,
            m02,
            m10,
            m11,
            m12,
            m20,
            m21,
            m22
        );
    }

    /// Convert to a 4x4 matrix by filling in an identity last row.
    Matrix4 ToMatrix4() const
    {
#ifdef MATH_SSE
        Matrix4 ret;
        _mm_storeu_ps(&ret.m00, _mm_loadu_ps(&m00));
        _mm_storeu_ps(&ret.m10, _mm_loadu_ps(&m10));
        _mm_storeu_ps(&ret.m20, _mm_loadu_ps(&m20));
        _mm_storeu_ps(&ret.m30, _mm_set_ps(1.f, 0.f, 0.f, 0.f));
        return ret;
#else

        return Matrix4(
            m00,
            m01,
            m02,
            m03,
            m10,
            m11,
            m12,
            m13,
            m20,
            m21,
            m22,
            m23,
            0.0f,
            0.0f,
            0.0f,
            1.0f
        );
#endif
    }

    /// Return the rotation matrix with scaling removed.
    Matrix3 RotationMatrix() const
    {
        Vector3 invScale(
            1.0f / sqrtf(m00 * m00 + m10 * m10 + m20 * m20),
            1.0f / sqrtf(m01 * m01 + m11 * m11 + m21 * m21),
            1.0f / sqrtf(m02 * m02 + m12 * m12 + m22 * m22)
        );

        return ToMatrix3().Scaled(invScale);
    }

    /// Return the translation part.
    Vector3 Translation() const
    {
        return Vector3(
            m03,
            m13,
            m23
        );
    }

    /// Return the rotation part.
    Quaternion Rotation() const { return Quaternion(RotationMatrix()); }

    /// Return the scaling part.
    Vector3 Scale() const
    {
        return Vector3(
            sqrtf(m00 * m00 + m10 * m10 + m20 * m20),
            sqrtf(m01 * m01 + m11 * m11 + m21 * m21),
            sqrtf(m02 * m02 + m12 * m12 + m22 * m22)
        );
    }

    /// Test for equality with another matrix with epsilon.
    bool Equals(const Matrix3x4& rhs) const
    {
        const float* leftData = Data();
        const float* rightData = rhs.Data();

        for (unsigned i = 0; i < 12; ++i)
        {
            if (!Math::Equals(leftData[i], rightData[i]))
                return false;
        }

        return true;
    }

    /// Return decomposition to translation, rotation and scale.
    void Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const;
    /// Return inverse.
    Matrix3x4 Inverse() const;

    /// Return float data.
    const float* Data() const { return &m00; }

    float m00;
    float m01;
    float m02;
    float m03;
    float m10;
    float m11;
    float m12;
    float m13;
    float m20;
    float m21;
    float m22;
    float m23;

    /// Zero matrix.
    static const Matrix3x4 ZERO;
    /// Identity matrix.
    static const Matrix3x4 IDENTITY;

#ifdef MATH_SSE
private:
    // Sets this matrix from the given translation, rotation (as quaternion (w,x,y,z)), and nonuniform scale (x,y,z) parameters.
    // Note: the w component of the scale parameter passed to this function must be 1.
    void inline SetFromTRS(__m128 t, __m128 q, __m128 s)
    {
        q = _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 2, 1));
        __m128 one = _mm_set_ps(0, 0, 0, 1);
        const __m128 sseX1 = _mm_castsi128_ps(_mm_set_epi32((int)0x80000000UL, (int)0x80000000UL, 0, (int)0x80000000UL));
        __m128 q2 = _mm_add_ps(q, q);
        __m128 t2 = _mm_add_ss(_mm_xor_ps(_mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(3, 3, 3, 2)), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 1, 2, 2))), sseX1), one);
        const __m128 sseX0 = _mm_shuffle_ps(sseX1, sseX1, _MM_SHUFFLE(0, 3, 2, 1));
        __m128 t0 = _mm_mul_ps(_mm_shuffle_ps(q, q, _MM_SHUFFLE(1, 0, 0, 1)), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(2, 2, 1, 1)));
        __m128 t1 = _mm_xor_ps(t0, sseX0);
        __m128 r0 = _mm_sub_ps(t2, t1);
        __m128 xx2 = _mm_mul_ss(q, q2);
        __m128 r1 = _mm_sub_ps(_mm_xor_ps(t2, sseX0), _mm_move_ss(t1, xx2));
        r1 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 r2 = _mm_shuffle_ps(_mm_movehl_ps(r0, r1), _mm_sub_ss(_mm_sub_ss(one, xx2), t0), _MM_SHUFFLE(2, 0, 3, 1));
        __m128 tmp0 = _mm_unpacklo_ps(r0, r1);
        __m128 tmp2 = _mm_unpacklo_ps(r2, t);
        __m128 tmp1 = _mm_unpackhi_ps(r0, r1);
        __m128 tmp3 = _mm_unpackhi_ps(r2, t);
        _mm_storeu_ps(&m00, _mm_mul_ps(_mm_movelh_ps(tmp0, tmp2), s));
        _mm_storeu_ps(&m10, _mm_mul_ps(_mm_movehl_ps(tmp2, tmp0), s));
        _mm_storeu_ps(&m20, _mm_mul_ps(_mm_movelh_ps(tmp1, tmp3), s));
    }
#endif
};

/// Multiply a 3x4 matrix with a scalar.
inline Matrix3x4 operator *(float lhs, const Matrix3x4& rhs) { return rhs * lhs; }


