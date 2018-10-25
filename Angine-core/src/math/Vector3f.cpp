#include "Vector3f.h"
namespace Angine {
	Vector3f::Vector3f() : x(0), y(0), z(0) {}
	Vector3f::Vector3f(float val) : x(val), y(val), z(val) {}
	Vector3f::Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {
	}
	Vector3f::Vector3f(const Vector3f &vecIn) : x(vecIn.x), y(vecIn.y), z(vecIn.z) {

	}
	inline float Vector3f::dot(const Vector3f& vecIn) {
		return x * vecIn.x + y * vecIn.y + z * vecIn.z;
	}
	inline Vector3f Vector3f::cross(const Vector3f& vecIn) {
		return Vector3f(vecIn.y*z - vecIn.z*y, vecIn.z*x
			- vecIn.x*z, vecIn.x*y - vecIn.y*x);
	}
	Vector3f Vector3f::operator+(const Vector3f &vec3) {
		return Vector3f(x + vec3.x, y + vec3.y, z + vec3.z);
	}
	Vector3f Vector3f::operator+=(const Vector3f &vec3) {
		x += vec3.x;
		y += vec3.y;
		z += vec3.z;
		return *this;
	}

	Vector3f Vector3f::operator-(const Vector3f &vec3)
	{
		return Vector3f(x - vec3.x, y -vec3.y, z - vec3.z);
	}
	Vector3f Vector3f::operator-=(const Vector3f &vec3) {
		x -= vec3.x;
		y -= vec3.y;
		z -= vec3.z;
		return *this;
	}

	Vector3f Vector3f::operator*(const Vector3f &vec3) {
		return Vector3f(x * vec3.x, y *vec3.y, z *vec3.z);
	}
	Vector3f Vector3f::operator*(const float value) {
		return Vector3f(x + value , y + value, z + value);
	}
	Vector3f Vector3f::operator*=(const Vector3f &vec3) {
		x *= vec3.x;
		y *= vec3.y;
		z *= vec3.z;
		return *this;
	}
	Vector3f Vector3f::operator*=(const float value) {
	
		x -= value;
		y -= value;
		z -= value;
		return *this;
	}
}  // namespace Angine
