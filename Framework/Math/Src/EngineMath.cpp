#include "Precompiled.h"
#include "EngineMath.h"

using namespace Angazi::Math;

//Definition
const Vector2 Vector2::Zero{ 0.0f };
const Vector2 Vector2::One{ 1.0f };
const Vector2 Vector2::XAxis{ 1.0f , 0.0f };
const Vector2 Vector2::YAxis{ 0.0f , 1.0f };

const Vector3 Vector3::Zero{ 0.0f };
const Vector3 Vector3::One{ 1.0f };
const Vector3 Vector3::XAxis{ 1.0f , 0.0f, 0.0f};
const Vector3 Vector3::YAxis{ 0.0f , 1.0f, 0.0f};
const Vector3 Vector3::ZAxis{ 0.0f , 0.0f, 1.0f};

//Declaration
const Vector4 Vector4::Zero{ 0.0f };
const Vector4 Vector4::One{ 1.0f };
const Vector4 Vector4::XAxis{ 1.0f , 0.0f, 0.0f, 0.0f};
const Vector4 Vector4::YAxis{ 0.0f , 1.0f, 0.0f, 0.0f};
const Vector4 Vector4::ZAxis{ 0.0f , 0.0f, 1.0f, 0.0f };
const Vector4 Vector4::WAxis{ 0.0f , 0.0f, 0.0f, 1.0f};

const Matrix4 Matrix4::Identity
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};