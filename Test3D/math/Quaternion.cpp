#include "Quaternion.h"

const Quaternion Quaternion::IDENTITY;

void Quaternion::FromAngleAxis(float angle, const Vector3& axis)
{
	Vector3 normAxis = axis.Normalized();
	angle *= Math::kDEG2RAD_2;
	float sinAngle = sinf(angle);
	float cosAngle = cosf(angle);

	w = cosAngle;
	x = normAxis.x * sinAngle;
	y = normAxis.y * sinAngle;
	z = normAxis.z * sinAngle;
}

void Quaternion::FromEulerAngles(float x, float y, float z)
{
	// Order of rotations: Z first, then X, then Y (mimics typical FPS camera with gimbal lock at top/bottom)
	x *= Math::kDEG2RAD_2;
	y *= Math::kDEG2RAD_2;
	z *= Math::kDEG2RAD_2;
	float sinX = sinf(x);
	float cosX = cosf(x);
	float sinY = sinf(y);
	float cosY = cosf(y);
	float sinZ = sinf(z);
	float cosZ = cosf(z);

	w = cosY * cosX * cosZ + sinY * sinX * sinZ;
	x = cosY * sinX * cosZ + sinY * cosX * sinZ;
	y = sinY * cosX * cosZ - cosY * sinX * sinZ;
	z = cosY * cosX * sinZ - sinY * sinX * cosZ;
}

void Quaternion::FromRotationTo(const Vector3& start, const Vector3& end)
{
	Vector3 normStart = start.Normalized();
	Vector3 normEnd = end.Normalized();
	float d = normStart.Dot(normEnd);

	if (d > -1.0f + Math::kLARGE_EPSILON) {
		Vector3 c = normStart.Cross(normEnd);
		float s = sqrtf((1.0f + d) * 2.0f);
		float invS = 1.0f / s;

		x = c.x * invS;
		y = c.y * invS;
		z = c.z * invS;
		w = 0.5f * s;
	}
	else {
		Vector3 axis = Vector3::RIGHT.Cross(normStart);
		if (axis.Length() < Math::kLARGE_EPSILON)
			axis = Vector3::UP.Cross(normStart);

		FromAngleAxis(180.f, axis);
	}
}

void Quaternion::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
{
	Matrix3 matrix(
		xAxis.x, yAxis.x, zAxis.x,
		xAxis.y, yAxis.y, zAxis.y,
		xAxis.z, yAxis.z, zAxis.z
		);

	FromRotationMatrix(matrix);
}

void Quaternion::FromRotationMatrix(const Matrix3& matrix)
{
	float t = matrix.m00 + matrix.m11 + matrix.m22;

	if (t > 0.0f) {
		float invS = 0.5f / sqrtf(1.0f + t);

		x = (matrix.m21 - matrix.m12) * invS;
		y = (matrix.m02 - matrix.m20) * invS;
		z = (matrix.m10 - matrix.m01) * invS;
		w = 0.25f / invS;
	}
	else {
		if (matrix.m00 > matrix.m11 && matrix.m00 > matrix.m22) {
			float invS = 0.5f / sqrtf(1.0f + matrix.m00 - matrix.m11 - matrix.m22);

			x = 0.25f / invS;
			y = (matrix.m01 + matrix.m10) * invS;
			z = (matrix.m20 + matrix.m02) * invS;
			w = (matrix.m21 - matrix.m12) * invS;
		}
		else if (matrix.m11 > matrix.m22) {
			float invS = 0.5f / sqrtf(1.0f + matrix.m11 - matrix.m00 - matrix.m22);

			x = (matrix.m01 + matrix.m10) * invS;
			y = 0.25f / invS;
			z = (matrix.m12 + matrix.m21) * invS;
			w = (matrix.m02 - matrix.m20) * invS;
		}
		else {
			float invS = 0.5f / sqrtf(1.0f + matrix.m22 - matrix.m00 - matrix.m11);

			x = (matrix.m02 + matrix.m20) * invS;
			y = (matrix.m12 + matrix.m21) * invS;
			z = 0.25f / invS;
			w = (matrix.m10 - matrix.m01) * invS;
		}
	}
}

bool Quaternion::FromLookRotation(const Vector3& direction, const Vector3& upDirection)
{
	Quaternion ret;
	Vector3 forward = direction.Normalized();

	Vector3 v = forward.Cross(upDirection);
	// If direction & upDirection are parallel and crossproduct becomes zero, use FromRotationTo() fallback
	if (v.LengthSquared() >= Math::kLARGE_EPSILON) {
		v.Normalize();
		Vector3 up = v.Cross(forward);
		Vector3 right = up.Cross(forward);
		ret.FromAxes(right, up, forward);
	}
	else
		ret.FromRotationTo(Vector3::FORWARD, forward);

	if (!ret.IsNaN()) {
		(*this) = ret;
		return true;
	}
	else
		return false;
}

Vector3 Quaternion::EulerAngles() const
{
	// Derivation from http://www.geometrictools.com/Documentation/EulerAngles.pdf
	// Order of rotations: Z first, then X, then Y
	float check = 2.0f * (-y * z + w * x);

	if (check < -0.995f) {
		return Vector3(
			-90.0f,
			0.0f,
			-atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)) * Math::kRAD2DEG
			);
	}
	else if (check > 0.995f) {
		return Vector3(
			90.0f,
			0.0f,
			atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)) * Math::kRAD2DEG
			);
	}
	else {
		return Vector3(
			asinf(check) * Math::kRAD2DEG,
			atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y)) * Math::kRAD2DEG,
			atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z)) * Math::kRAD2DEG
			);
	}
}

float Quaternion::YawAngle() const
{
	return EulerAngles().y;
}

float Quaternion::PitchAngle() const
{
	return EulerAngles().x;
}

float Quaternion::RollAngle() const
{
	return EulerAngles().z;
}

Matrix3 Quaternion::RotationMatrix() const
{
	return Matrix3(
		1.0f - 2.0f * y * y - 2.0f * z * z,
		2.0f * x * y - 2.0f * w * z,
		2.0f * x * z + 2.0f * w * y,
		2.0f * x * y + 2.0f * w * z,
		1.0f - 2.0f * x * x - 2.0f * z * z,
		2.0f * y * z - 2.0f * w * x,
		2.0f * x * z - 2.0f * w * y,
		2.0f * y * z + 2.0f * w * x,
		1.0f - 2.0f * x * x - 2.0f * y * y
		);
}

Quaternion Quaternion::Slerp(Quaternion rhs, float t) const
{
	// Use fast approximation for Emscripten builds
#ifdef __EMSCRIPTEN__
	float angle = Dot(rhs);
	float sign = 1.f; // Multiply by a sign of +/-1 to guarantee we rotate the shorter arc.
	if (angle < 0.f) {
		angle = -angle;
		sign = -1.f;
	}

	float a;
	float b;
	if (angle < 0.999f) // perform spherical linear interpolation.
	{
		// angle = acos(angle); // After this, angle is in the range pi/2 -> 0 as the original angle variable ranged from 0 -> 1.
		angle = (-0.69813170079773212f * angle * angle - 0.87266462599716477f) * angle + 1.5707963267948966f;
		float ta = t*angle;
		// Manually compute the two sines by using a very rough approximation.
		float ta2 = ta*ta;
		b = ((5.64311797634681035370e-03f * ta2 - 1.55271410633428644799e-01f) * ta2 + 9.87862135574673806965e-01f) * ta;
		a = angle - ta;
		float a2 = a*a;
		a = ((5.64311797634681035370e-03f * a2 - 1.55271410633428644799e-01f) * a2 + 9.87862135574673806965e-01f) * a;
	}
	else // If angle is close to taking the denominator to zero, resort to linear interpolation (and normalization).
	{
		a = 1.f - t;
		b = t;
	}
	// Lerp and renormalize.
	return (*this * (a * sign) + rhs * b).Normalized();
#else
	// Favor accuracy for native code builds
	float cosAngle = Dot(rhs);
	// Enable shortest path rotation
	if (cosAngle < 0.0f) {
		cosAngle = -cosAngle;
		rhs = -rhs;
	}

	float angle = acosf(cosAngle);
	float sinAngle = sinf(angle);
	float t1, t2;

	if (sinAngle > 0.001f) {
		float invSinAngle = 1.0f / sinAngle;
		t1 = sinf((1.0f - t) * angle) * invSinAngle;
		t2 = sinf(t * angle) * invSinAngle;
	}
	else {
		t1 = 1.0f - t;
		t2 = t;
	}

	return *this * t1 + rhs * t2;
#endif
}

Quaternion Quaternion::Nlerp(Quaternion rhs, float t, bool shortestPath) const
{
	Quaternion result;
	float fCos = Dot(rhs);
	if (fCos < 0.0f && shortestPath)
		result = (*this) + (((-rhs) - (*this)) * t);
	else
		result = (*this) + ((rhs - (*this)) * t);
	result.Normalize();
	return result;
}
