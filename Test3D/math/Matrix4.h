#pragma once

#include "Math.h"
#include "Quaternion.h"
#include "Vector4.h"

#ifdef MATH_SSE
#include <emmintrin.h>
#endif

class Matrix4
{
public:
    
    Matrix4()
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
        m23(0.0f),
        m30(0.0f),
        m31(0.0f),
        m32(0.0f),
        m33(1.0f)
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&m00, _mm_set_ps(0.f, 0.f, 0.f, 1.f));
        _mm_storeu_ps(&m10, _mm_set_ps(0.f, 0.f, 1.f, 0.f));
        _mm_storeu_ps(&m20, _mm_set_ps(0.f, 1.f, 0.f, 0.f));
        _mm_storeu_ps(&m30, _mm_set_ps(1.f, 0.f, 0.f, 0.f));
#endif
    }

    
    Matrix4(const Matrix4& matrix)
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
        m23(matrix.m23),
        m30(matrix.m30),
        m31(matrix.m31),
        m32(matrix.m32),
        m33(matrix.m33)
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&m00, _mm_loadu_ps(&matrix.m00));
        _mm_storeu_ps(&m10, _mm_loadu_ps(&matrix.m10));
        _mm_storeu_ps(&m20, _mm_loadu_ps(&matrix.m20));
        _mm_storeu_ps(&m30, _mm_loadu_ps(&matrix.m30));
#endif
    }

    
    Matrix4(const Matrix3& matrix) :
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
        m23(0.0f),
        m30(0.0f),
        m31(0.0f),
        m32(0.0f),
        m33(1.0f)
    {
    }

    // Construct from values.
    Matrix4(float v00, float v01, float v02, float v03,
            float v10, float v11, float v12, float v13,
            float v20, float v21, float v22, float v23,
            float v30, float v31, float v32, float v33) :
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
        m23(v23),
        m30(v30),
        m31(v31),
        m32(v32),
        m33(v33)
    {
    }

    
    explicit Matrix4(const float* data)
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
        m23(data[11]),
        m30(data[12]),
        m31(data[13]),
        m32(data[14]),
        m33(data[15])
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&m00, _mm_loadu_ps(data));
        _mm_storeu_ps(&m10, _mm_loadu_ps(data + 4));
        _mm_storeu_ps(&m20, _mm_loadu_ps(data + 8));
        _mm_storeu_ps(&m30, _mm_loadu_ps(data + 12));
#endif
    }

    
    Matrix4& operator =(const Matrix4& rhs)
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&m00, _mm_loadu_ps(&rhs.m00));
        _mm_storeu_ps(&m10, _mm_loadu_ps(&rhs.m10));
        _mm_storeu_ps(&m20, _mm_loadu_ps(&rhs.m20));
        _mm_storeu_ps(&m30, _mm_loadu_ps(&rhs.m30));
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
        m30 = rhs.m30;
        m31 = rhs.m31;
        m32 = rhs.m32;
        m33 = rhs.m33;
#endif
        return *this;
    }

    
    Matrix4& operator =(const Matrix3& rhs)
    {
        m00 = rhs.m00;
        m01 = rhs.m01;
        m02 = rhs.m02;
        m03 = 0.0f;
        m10 = rhs.m10;
        m11 = rhs.m11;
        m12 = rhs.m12;
        m13 = 0.0f;
        m20 = rhs.m20;
        m21 = rhs.m21;
        m22 = rhs.m22;
        m23 = 0.0f;
        m30 = 0.0f;
        m31 = 0.0f;
        m32 = 0.0f;
        m33 = 1.0f;
        return *this;
    }

    
    bool operator ==(const Matrix4& rhs) const
    {
#ifdef MATH_SSE
        __m128 c0 = _mm_cmpeq_ps(_mm_loadu_ps(&m00), _mm_loadu_ps(&rhs.m00));
        __m128 c1 = _mm_cmpeq_ps(_mm_loadu_ps(&m10), _mm_loadu_ps(&rhs.m10));
        c0 = _mm_and_ps(c0, c1);
        __m128 c2 = _mm_cmpeq_ps(_mm_loadu_ps(&m20), _mm_loadu_ps(&rhs.m20));
        __m128 c3 = _mm_cmpeq_ps(_mm_loadu_ps(&m30), _mm_loadu_ps(&rhs.m30));
        c2 = _mm_and_ps(c2, c3);
        c0 = _mm_and_ps(c0, c2);
        __m128 hi = _mm_movehl_ps(c0, c0);
        c0 = _mm_and_ps(c0, hi);
        hi = _mm_shuffle_ps(c0, c0, _MM_SHUFFLE(1, 1, 1, 1));
        c0 = _mm_and_ps(c0, hi);
        return !_mm_ucomige_ss(c0, c0);
#else
        const float* leftData = Data();
        const float* rightData = rhs.Data();

        for (unsigned i = 0; i < 16; ++i)
        {
            if (leftData[i] != rightData[i])
                return false;
        }

        return true;
#endif
    }

    
    bool operator !=(const Matrix4& rhs) const { return !(*this == rhs); }

    
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
        __m128 r3 = _mm_mul_ps(_mm_loadu_ps(&m30), vec);
        __m128 t2 = _mm_unpacklo_ps(r2, r3);
        __m128 t3 = _mm_unpackhi_ps(r2, r3);
        t2 = _mm_add_ps(t2, t3);
        vec = _mm_add_ps(_mm_movelh_ps(t0, t2), _mm_movehl_ps(t2, t0));
        vec = _mm_div_ps(vec, _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(3, 3, 3, 3)));
        return Vector3(
            _mm_cvtss_f32(vec),
            _mm_cvtss_f32(_mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 1, 1, 1))),
            _mm_cvtss_f32(_mm_movehl_ps(vec, vec)));
#else
        float invW = 1.0f / (m30 * rhs.x + m31 * rhs.y + m32 * rhs.z + m33);

        return Vector3(
            (m00 * rhs.x + m01 * rhs.y + m02 * rhs.z + m03) * invW,
            (m10 * rhs.x + m11 * rhs.y + m12 * rhs.z + m13) * invW,
            (m20 * rhs.x + m21 * rhs.y + m22 * rhs.z + m23) * invW
        );
#endif
    }

    
    Vector4 operator *(const Vector4& rhs) const
    {
#ifdef MATH_SSE
        __m128 vec = _mm_loadu_ps(&rhs.x);
        __m128 r0 = _mm_mul_ps(_mm_loadu_ps(&m00), vec);
        __m128 r1 = _mm_mul_ps(_mm_loadu_ps(&m10), vec);
        __m128 t0 = _mm_unpacklo_ps(r0, r1);
        __m128 t1 = _mm_unpackhi_ps(r0, r1);
        t0 = _mm_add_ps(t0, t1);
        __m128 r2 = _mm_mul_ps(_mm_loadu_ps(&m20), vec);
        __m128 r3 = _mm_mul_ps(_mm_loadu_ps(&m30), vec);
        __m128 t2 = _mm_unpacklo_ps(r2, r3);
        __m128 t3 = _mm_unpackhi_ps(r2, r3);
        t2 = _mm_add_ps(t2, t3);
        vec = _mm_add_ps(_mm_movelh_ps(t0, t2), _mm_movehl_ps(t2, t0));

        Vector4 ret;
        _mm_storeu_ps(&ret.x, vec);
        return ret;
#else
        return Vector4(
            m00 * rhs.x + m01 * rhs.y + m02 * rhs.z + m03 * rhs.w,
            m10 * rhs.x + m11 * rhs.y + m12 * rhs.z + m13 * rhs.w,
            m20 * rhs.x + m21 * rhs.y + m22 * rhs.z + m23 * rhs.w,
            m30 * rhs.x + m31 * rhs.y + m32 * rhs.z + m33 * rhs.w
        );
#endif
    }

    
    Matrix4 operator +(const Matrix4& rhs) const
    {
#ifdef MATH_SSE
        Matrix4 ret;
        _mm_storeu_ps(&ret.m00, _mm_add_ps(_mm_loadu_ps(&m00), _mm_loadu_ps(&rhs.m00)));
        _mm_storeu_ps(&ret.m10, _mm_add_ps(_mm_loadu_ps(&m10), _mm_loadu_ps(&rhs.m10)));
        _mm_storeu_ps(&ret.m20, _mm_add_ps(_mm_loadu_ps(&m20), _mm_loadu_ps(&rhs.m20)));
        _mm_storeu_ps(&ret.m30, _mm_add_ps(_mm_loadu_ps(&m30), _mm_loadu_ps(&rhs.m30)));
        return ret;
#else
        return Matrix4(
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
            m23 + rhs.m23,
            m30 + rhs.m30,
            m31 + rhs.m31,
            m32 + rhs.m32,
            m33 + rhs.m33
        );
#endif
    }

    
    Matrix4 operator -(const Matrix4& rhs) const
    {
#ifdef MATH_SSE
        Matrix4 ret;
        _mm_storeu_ps(&ret.m00, _mm_sub_ps(_mm_loadu_ps(&m00), _mm_loadu_ps(&rhs.m00)));
        _mm_storeu_ps(&ret.m10, _mm_sub_ps(_mm_loadu_ps(&m10), _mm_loadu_ps(&rhs.m10)));
        _mm_storeu_ps(&ret.m20, _mm_sub_ps(_mm_loadu_ps(&m20), _mm_loadu_ps(&rhs.m20)));
        _mm_storeu_ps(&ret.m30, _mm_sub_ps(_mm_loadu_ps(&m30), _mm_loadu_ps(&rhs.m30)));
        return ret;
#else
        return Matrix4(
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
            m23 - rhs.m23,
            m30 - rhs.m30,
            m31 - rhs.m31,
            m32 - rhs.m32,
            m33 - rhs.m33
        );
#endif
    }

    
    Matrix4 operator *(float rhs) const
    {
#ifdef MATH_SSE
        Matrix4 ret;
        const __m128 mul = _mm_set1_ps(rhs);
        _mm_storeu_ps(&ret.m00, _mm_mul_ps(_mm_loadu_ps(&m00), mul));
        _mm_storeu_ps(&ret.m10, _mm_mul_ps(_mm_loadu_ps(&m10), mul));
        _mm_storeu_ps(&ret.m20, _mm_mul_ps(_mm_loadu_ps(&m20), mul));
        _mm_storeu_ps(&ret.m30, _mm_mul_ps(_mm_loadu_ps(&m30), mul));
        return ret;
#else
        return Matrix4(
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
            m23 * rhs,
            m30 * rhs,
            m31 * rhs,
            m32 * rhs,
            m33 * rhs
        );
#endif
    }

    
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

        l = _mm_loadu_ps(&m30);
        t0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
        t1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
        t2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
        t3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
        _mm_storeu_ps(&out.m30, _mm_add_ps(_mm_add_ps(t0, t1), _mm_add_ps(t2, t3)));

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
            m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30,
            m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31,
            m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32,
            m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33
        );
#endif
    }

    void SetTranslation(const Vector3& translation)
    {
        m03 = translation.x;
        m13 = translation.y;
        m23 = translation.z;
    }

    
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

    // Set scaling elements.
    void SetScale(const Vector3& scale)
    {
        m00 = scale.x;
        m11 = scale.y;
        m22 = scale.z;
    }

    // Set uniform scaling elements.
    void SetScale(float scale)
    {
        m00 = scale;
        m11 = scale;
        m22 = scale;
    }

    
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

    
    Matrix3 RotationMatrix() const
    {
        Vector3 invScale(
            1.0f / sqrtf(m00 * m00 + m10 * m10 + m20 * m20),
            1.0f / sqrtf(m01 * m01 + m11 * m11 + m21 * m21),
            1.0f / sqrtf(m02 * m02 + m12 * m12 + m22 * m22)
        );

        return ToMatrix3().Scaled(invScale);
    }

    
    Vector3 Translation() const
    {
        return Vector3(
            m03,
            m13,
            m23
        );
    }

    
    Quaternion Rotation() const { return Quaternion(RotationMatrix()); }

    
    Vector3 Scale() const
    {
        return Vector3(
            sqrtf(m00 * m00 + m10 * m10 + m20 * m20),
            sqrtf(m01 * m01 + m11 * m11 + m21 * m21),
            sqrtf(m02 * m02 + m12 * m12 + m22 * m22)
        );
    }

    
    Matrix4 Transpose() const
    {
#ifdef MATH_SSE
        __m128 m0 = _mm_loadu_ps(&m00);
        __m128 m1 = _mm_loadu_ps(&m10);
        __m128 m2 = _mm_loadu_ps(&m20);
        __m128 m3 = _mm_loadu_ps(&m30);
        _MM_TRANSPOSE4_PS(m0, m1, m2, m3);
        Matrix4 out;
        _mm_storeu_ps(&out.m00, m0);
        _mm_storeu_ps(&out.m10, m1);
        _mm_storeu_ps(&out.m20, m2);
        _mm_storeu_ps(&out.m30, m3);
        return out;
#else
        return Matrix4(
            m00,
            m10,
            m20,
            m30,
            m01,
            m11,
            m21,
            m31,
            m02,
            m12,
            m22,
            m32,
            m03,
            m13,
            m23,
            m33
        );
#endif
    }

    
    bool Equals(const Matrix4& rhs) const
    {
        const float* leftData = Data();
        const float* rightData = rhs.Data();

        for (unsigned i = 0; i < 16; ++i)
        {
            if (!Math::Equals(leftData[i], rightData[i]))
                return false;
        }

        return true;
    }

    
    void Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const;
    
    Matrix4 Inverse() const;

    
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
    float m30;
    float m31;
    float m32;
    float m33;

    
    static void BulkTranspose(float* dest, const float* src, unsigned count)
    {
        for (unsigned i = 0; i < count; ++i)
        {
#ifdef MATH_SSE
            __m128 m0 = _mm_loadu_ps(src);
            __m128 m1 = _mm_loadu_ps(src + 4);
            __m128 m2 = _mm_loadu_ps(src + 8);
            __m128 m3 = _mm_loadu_ps(src + 12);
            _MM_TRANSPOSE4_PS(m0, m1, m2, m3);
            _mm_storeu_ps(dest, m0);
            _mm_storeu_ps(dest + 4, m1);
            _mm_storeu_ps(dest + 8, m2);
            _mm_storeu_ps(dest + 12, m3);
#else
            dest[0] = src[0];
            dest[1] = src[4];
            dest[2] = src[8];
            dest[3] = src[12];
            dest[4] = src[1];
            dest[5] = src[5];
            dest[6] = src[9];
            dest[7] = src[13];
            dest[8] = src[2];
            dest[9] = src[6];
            dest[10] = src[10];
            dest[11] = src[14];
            dest[12] = src[3];
            dest[13] = src[7];
            dest[14] = src[11];
            dest[15] = src[15];
#endif
            dest += 16;
            src += 16;
        }
    }

    
    static const Matrix4 ZERO;
    
    static const Matrix4 IDENTITY;
};


inline Matrix4 operator *(float lhs, const Matrix4& rhs) { return rhs * lhs; }
