#pragma once

#include "Matrix3.h"

#ifdef MATH_SSE
#include <emmintrin.h>
#endif

class Quaternion
{
public:
    
    Quaternion()
#ifndef MATH_SSE
       :w(1.0f),
        x(0.0f),
        y(0.0f),
        z(0.0f)
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&w, _mm_set_ps(0.f, 0.f, 0.f, 1.f));
#endif
    }

    
    Quaternion(const Quaternion& quat)
#ifndef MATH_SSE
       :w(quat.w),
        x(quat.x),
        y(quat.y),
        z(quat.z)
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&w, _mm_loadu_ps(&quat.w));
#endif
    }

    
    Quaternion(float w, float x, float y, float z)
#ifndef MATH_SSE
       :w(w),
        x(x),
        y(y),
        z(z)
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&w, _mm_set_ps(z, y, x, w));
#endif
    }

    
    explicit Quaternion(const float* data)
#ifndef MATH_SSE
       :w(data[0]),
        x(data[1]),
        y(data[2]),
        z(data[3])
#endif
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&w, _mm_loadu_ps(data));
#endif
    }

    
    Quaternion(float angle, const Vector3& axis)
    {
        FromAngleAxis(angle, axis);
    }

    
    explicit Quaternion(float angle)
    {
        FromAngleAxis(angle, Vector3::FORWARD);
    }

    
    Quaternion(float x, float y, float z)
    {
        FromEulerAngles(x, y, z);
    }

    
    Quaternion(const Vector3& start, const Vector3& end)
    {
        FromRotationTo(start, end);
    }

    
    Quaternion(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
    {
        FromAxes(xAxis, yAxis, zAxis);
    }

    
    explicit Quaternion(const Matrix3& matrix)
    {
        FromRotationMatrix(matrix);
    }

#ifdef MATH_SSE
    explicit Quaternion(__m128 wxyz)
    {
        _mm_storeu_ps(&w, wxyz);
    }
#endif

    
    Quaternion& operator =(const Quaternion& rhs)
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&w, _mm_loadu_ps(&rhs.w));
#else
        w = rhs.w;
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
#endif
        return *this;
    }

    
    Quaternion& operator +=(const Quaternion& rhs)
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&w, _mm_add_ps(_mm_loadu_ps(&w), _mm_loadu_ps(&rhs.w)));
#else
        w += rhs.w;
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
#endif
        return *this;
    }

    
    Quaternion& operator *=(float rhs)
    {
#ifdef MATH_SSE
        _mm_storeu_ps(&w, _mm_mul_ps(_mm_loadu_ps(&w), _mm_set1_ps(rhs)));
#else
        w *= rhs;
        x *= rhs;
        y *= rhs;
        z *= rhs;
#endif
        return *this;
    }

    
    bool operator ==(const Quaternion& rhs) const
    {
#ifdef MATH_SSE
        __m128 c = _mm_cmpeq_ps(_mm_loadu_ps(&w), _mm_loadu_ps(&rhs.w));
        c = _mm_and_ps(c, _mm_movehl_ps(c, c));
        c = _mm_and_ps(c, _mm_shuffle_ps(c, c, _MM_SHUFFLE(1, 1, 1, 1)));
        return !_mm_ucomige_ss(c, c);
#else
        return w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z;
#endif
    }

    
    bool operator !=(const Quaternion& rhs) const { return !(*this == rhs); }

    
    Quaternion operator *(float rhs) const
    {
#ifdef MATH_SSE
        return Quaternion(_mm_mul_ps(_mm_loadu_ps(&w), _mm_set1_ps(rhs)));
#else
        return Quaternion(w * rhs, x * rhs, y * rhs, z * rhs);
#endif
    }

    
    Quaternion operator -() const
    {
#ifdef MATH_SSE
        return Quaternion(_mm_xor_ps(_mm_loadu_ps(&w), _mm_castsi128_ps(_mm_set1_epi32((int)0x80000000UL))));
#else
        return Quaternion(-w, -x, -y, -z);
#endif
    }

    
    Quaternion operator +(const Quaternion& rhs) const
    {
#ifdef MATH_SSE
        return Quaternion(_mm_add_ps(_mm_loadu_ps(&w), _mm_loadu_ps(&rhs.w)));
#else
        return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
#endif
    }

    
    Quaternion operator -(const Quaternion& rhs) const
    {
#ifdef MATH_SSE
        return Quaternion(_mm_sub_ps(_mm_loadu_ps(&w), _mm_loadu_ps(&rhs.w)));
#else
        return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
#endif
    }

    
    Quaternion operator *(const Quaternion& rhs) const
    {
#ifdef MATH_SSE
        __m128 q1 = _mm_loadu_ps(&w);
        __m128 q2 = _mm_loadu_ps(&rhs.w);
        q2 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 3, 2, 1));
        const __m128 signy = _mm_castsi128_ps(_mm_set_epi32((int)0x80000000UL, (int)0x80000000UL, 0, 0));
        const __m128 signx = _mm_shuffle_ps(signy, signy, _MM_SHUFFLE(2, 0, 2, 0));
        const __m128 signz = _mm_shuffle_ps(signy, signy, _MM_SHUFFLE(3, 0, 0, 3));
        __m128 out = _mm_mul_ps(_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(1, 1, 1, 1)), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(0, 1, 2, 3)));
        out = _mm_add_ps(_mm_mul_ps(_mm_xor_ps(signy, _mm_shuffle_ps(q1, q1, _MM_SHUFFLE(2, 2, 2, 2))), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(1, 0, 3, 2))), _mm_xor_ps(signx, out));
        out = _mm_add_ps(_mm_mul_ps(_mm_xor_ps(signz, _mm_shuffle_ps(q1, q1, _MM_SHUFFLE(3, 3, 3, 3))), _mm_shuffle_ps(q2, q2, _MM_SHUFFLE(2, 3, 0, 1))), out);
        out = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(q1, q1, _MM_SHUFFLE(0, 0, 0, 0)), q2), out);
        return Quaternion(_mm_shuffle_ps(out, out, _MM_SHUFFLE(2, 1, 0, 3)));
#else
        return Quaternion(
            w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
            w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
            w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
            w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x
        );
#endif
    }

    
    Vector3 operator *(const Vector3& rhs) const
    {
#ifdef MATH_SSE
        __m128 q = _mm_loadu_ps(&w);
        q = _mm_shuffle_ps(q, q, _MM_SHUFFLE(0, 3, 2, 1));
        __m128 v = _mm_set_ps(0.f, rhs.z, rhs.y, rhs.x);
        const __m128 W = _mm_shuffle_ps(q, q, _MM_SHUFFLE(3, 3, 3, 3));
        const __m128 a_yzx = _mm_shuffle_ps(q, q, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 x = _mm_mul_ps(q, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)));
        __m128 qxv = _mm_sub_ps(x, _mm_mul_ps(a_yzx, v));
        __m128 Wv = _mm_mul_ps(W, v);
        __m128 s = _mm_add_ps(qxv, _mm_shuffle_ps(Wv, Wv, _MM_SHUFFLE(3, 1, 0, 2)));
        __m128 qs = _mm_mul_ps(q, s);
        __m128 y = _mm_shuffle_ps(qs, qs, _MM_SHUFFLE(3, 1, 0, 2));
        s = _mm_sub_ps(_mm_mul_ps(a_yzx, s), y);
        s = _mm_add_ps(s, s);
        s = _mm_add_ps(s, v);

        return Vector3(
            _mm_cvtss_f32(s),
            _mm_cvtss_f32(_mm_shuffle_ps(s, s, _MM_SHUFFLE(1, 1, 1, 1))),
            _mm_cvtss_f32(_mm_movehl_ps(s, s)));
#else
        Vector3 qVec(x, y, z);
        Vector3 cross1(qVec.Cross(rhs));
        Vector3 cross2(qVec.Cross(cross1));

        return rhs + 2.0f * (cross1 * w + cross2);
#endif
    }

    
    void FromAngleAxis(float angle, const Vector3& axis);
    
    void FromEulerAngles(float x, float y, float z);
    
    void FromRotationTo(const Vector3& start, const Vector3& end);
    
    void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
    
    void FromRotationMatrix(const Matrix3& matrix);
    
    bool FromLookRotation(const Vector3& direction, const Vector3& up = Vector3::UP);

    
    void Normalize()
    {
#ifdef MATH_SSE
        __m128 q = _mm_loadu_ps(&w);
        __m128 n = _mm_mul_ps(q, q);
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
        __m128 e = _mm_rsqrt_ps(n);
        __m128 e3 = _mm_mul_ps(_mm_mul_ps(e, e), e);
        __m128 half = _mm_set1_ps(0.5f);
        n = _mm_add_ps(e, _mm_mul_ps(half, _mm_sub_ps(e, _mm_mul_ps(n, e3))));
        _mm_storeu_ps(&w, _mm_mul_ps(q, n));
#else
        float lenSquared = LengthSquared();
        if (!Math::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            w *= invLen;
            x *= invLen;
            y *= invLen;
            z *= invLen;
        }
#endif
    }

    
    Quaternion Normalized() const
    {
#ifdef MATH_SSE
        __m128 q = _mm_loadu_ps(&w);
        __m128 n = _mm_mul_ps(q, q);
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
        __m128 e = _mm_rsqrt_ps(n);
        __m128 e3 = _mm_mul_ps(_mm_mul_ps(e, e), e);
        __m128 half = _mm_set1_ps(0.5f);
        n = _mm_add_ps(e, _mm_mul_ps(half, _mm_sub_ps(e, _mm_mul_ps(n, e3))));
        return Quaternion(_mm_mul_ps(q, n));
#else
        float lenSquared = LengthSquared();
        if (!Math::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            return *this * invLen;
        }
        else
            return *this;
#endif
    }

    
    Quaternion Inverse() const
    {
#ifdef MATH_SSE
        __m128 q = _mm_loadu_ps(&w);
        __m128 n = _mm_mul_ps(q, q);
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
        return Quaternion(_mm_div_ps(_mm_xor_ps(q, _mm_castsi128_ps(_mm_set_epi32((int)0x80000000UL, (int)0x80000000UL, (int)0x80000000UL, 0))), n));
#else
        float lenSquared = LengthSquared();
        if (lenSquared == 1.0f)
            return Conjugate();
        else if (lenSquared >= Math::kLARGE_EPSILON)
            return Conjugate() * (1.0f / lenSquared);
        else
            return IDENTITY;
#endif
    }

    
    float LengthSquared() const
    {
#ifdef MATH_SSE
        __m128 q = _mm_loadu_ps(&w);
        __m128 n = _mm_mul_ps(q, q);
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
        return _mm_cvtss_f32(n);
#else
        return w * w + x * x + y * y + z * z;
#endif
    }

    
    float Dot(const Quaternion& rhs) const
    {
#ifdef MATH_SSE
        __m128 q1 = _mm_loadu_ps(&w);
        __m128 q2 = _mm_loadu_ps(&rhs.w);
        __m128 n = _mm_mul_ps(q1, q2);
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
        n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
        return _mm_cvtss_f32(n);
#else
        return w * rhs.w + x * rhs.x + y * rhs.y + z * rhs.z;
#endif
    }

    
    bool Equals(const Quaternion& rhs) const
    {
        return Math::Equals(w, rhs.w) && Math::Equals(x, rhs.x) && Math::Equals(y, rhs.y) && Math::Equals(z, rhs.z);
    }

    
    bool IsNaN() const { return Math::IsNaN(w) || Math::IsNaN(x) || Math::IsNaN(y) || Math::IsNaN(z); }

    
    Quaternion Conjugate() const
    {
#ifdef MATH_SSE
        __m128 q = _mm_loadu_ps(&w);
        return Quaternion(_mm_xor_ps(q, _mm_castsi128_ps(_mm_set_epi32((int)0x80000000UL, (int)0x80000000UL, (int)0x80000000UL, 0))));
#else
        return Quaternion(w, -x, -y, -z);
#endif
    }

    
    Vector3 EulerAngles() const;
    
    float YawAngle() const;
    
    float PitchAngle() const;
    
    float RollAngle() const;
    
    Matrix3 RotationMatrix() const;
    
    Quaternion Slerp(Quaternion rhs, float t) const;
    
    Quaternion Nlerp(Quaternion rhs, float t, bool shortestPath = false) const;

    
    const float* Data() const { return &w; }
    
    float w;
    float x;
    float y;
    float z;
    
    static const Quaternion IDENTITY;
};


