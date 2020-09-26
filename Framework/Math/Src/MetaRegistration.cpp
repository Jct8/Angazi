#include "Precompiled.h"
#include "MetaRegistration.h"

#include "Constants.h"
#include "Vector3.h"

using namespace Angazi::Math;

namespace Angazi::Core::Meta
{
	template<>
	void Deserialize<Vector2>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vec2 = (Vector2*)(instance);
		vec2->x = object.FindMember("x")->value.GetFloat();
		vec2->y = object.FindMember("y")->value.GetFloat();
	}
	template<>
	void Deserialize<Vector3>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vec3 = (Vector3*)(instance);
		vec3->x = object.FindMember("x")->value.GetFloat();
		vec3->y = object.FindMember("y")->value.GetFloat();
		vec3->z = object.FindMember("z")->value.GetFloat();
	}
	template<>
	void Deserialize<Vector4>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		auto vec4 = (Vector4*)(instance);
		vec4->x = object.FindMember("x")->value.GetFloat();
		vec4->y = object.FindMember("y")->value.GetFloat();
		vec4->z = object.FindMember("z")->value.GetFloat();
		vec4->w = object.FindMember("w")->value.GetFloat();
	}
	template<>
	void Deserialize<Quaternion>(void* instance, const rapidjson::Value& value)
	{
		auto object = value.GetObjectW();
		float eulerX = object.FindMember("xDegree")->value.GetFloat();
		float eulerY = object.FindMember("yDegree")->value.GetFloat();
		float eulerZ = object.FindMember("zDegree")->value.GetFloat();

		auto quat = (Quaternion*)(instance);
		*quat =
			Quaternion::RotationAxis(Vector3::XAxis, eulerX * Constants::DegToRad) *
			Quaternion::RotationAxis(Vector3::YAxis, eulerY * Constants::DegToRad) *
			Quaternion::RotationAxis(Vector3::ZAxis, eulerZ * Constants::DegToRad);
	}

	template<>
	void Serialize<Vector2>(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		auto vec2 = (Vector2*)(instance);
		value.AddMember("x", vec2->x, document.GetAllocator());
		value.AddMember("y", vec2->y, document.GetAllocator());
	}
	template<>
	void Serialize<Vector3>(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		auto vec3 = (Vector3*)(instance);
		value.AddMember("x", vec3->x, document.GetAllocator());
		value.AddMember("y", vec3->y, document.GetAllocator());
		value.AddMember("z", vec3->z, document.GetAllocator());
	}
	template<>
	void Serialize<Vector4>(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		auto vec4 = (Vector4*)(instance);
		value.AddMember("x", vec4->x, document.GetAllocator());
		value.AddMember("y", vec4->y, document.GetAllocator());
		value.AddMember("z", vec4->z, document.GetAllocator());
		value.AddMember("w", vec4->w, document.GetAllocator());
	}
	template<>
	void Serialize<Quaternion>(const void* instance, rapidjson::Value& value, rapidjson::Document& document)
	{
		auto quat = (Quaternion*)(instance);

		Math::Vector3 eulerAngle = Vector3::ToEulerAngleXYZ(*quat);
		value.AddMember("xDegree", eulerAngle.x * Constants::RadToDeg, document.GetAllocator());
		value.AddMember("yDegree", eulerAngle.y * Constants::RadToDeg, document.GetAllocator());
		value.AddMember("zDegree", eulerAngle.z * Constants::RadToDeg, document.GetAllocator());
	}
}

// Primitive Type Declarations
META_TYPE_DEFINE(Vector2, Vector2)
META_TYPE_DEFINE(Vector3, Vector3)
META_TYPE_DEFINE(Vector4, Vector4)
META_TYPE_DEFINE(Quaternion, Quaternion)

void Angazi::Math::StaticMetaRegister()
{
	// Add MetaClass registration here...
	// eg. META_REGISTER(Foo);
}
