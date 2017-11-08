#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

const Vector2 Vector2::ZERO;
const Vector2 Vector2::LEFT(-1.0f, 0.0f);
const Vector2 Vector2::RIGHT(1.0f, 0.0f);
const Vector2 Vector2::UP(0.0f, 1.0f);
const Vector2 Vector2::DOWN(0.0f, -1.0f);
const Vector2 Vector2::ONE(1.0f, 1.0f);


const Vector3 Vector3::ZERO;
const Vector3 Vector3::LEFT(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::RIGHT(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UP(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::DOWN(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::FORWARD(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::BACK(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::ONE(1.0f, 1.0f, 1.0f);

const Vector4 Vector4::ZERO;
const Vector4 Vector4::ONE(1.0f, 1.0f, 1.0f, 1.0f);