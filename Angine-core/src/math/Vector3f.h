#pragma once
#include <cmath>
namespace Angine {
class Vector3f {
 public:
  Vector3f();
  Vector3f(float val);
  Vector3f(float _x, float _y, float _z);
  Vector3f(const Vector3f &vecIn);
  inline float dot(const Vector3f &vecIn)const ;
  inline Vector3f cross(const Vector3f &vecIn) const  ;
  inline  Vector3f normalize()const  ;
  inline float length()const;
  Vector3f operator+(const Vector3f &vec3)const ;
  Vector3f& operator+=(const Vector3f &vec3);

  Vector3f operator-(const Vector3f &vec3)const ;
  Vector3f& operator-=(const Vector3f &vec3);

  Vector3f operator*(const Vector3f &vec3)const ;
  Vector3f operator*(const float value)const ;
  Vector3f& operator*=(const Vector3f &vec3);
  Vector3f& operator*=(const float value);

  Vector3f& operator/=(float);
  Vector3f operator/(float) const ;
  union {
	  struct { float x, y, z; };
    float v[3];
  };
};


inline float Vector3f::dot(const Vector3f& vecIn)const {
	return x * vecIn.x + y * vecIn.y + z * vecIn.z;
}
inline Vector3f Vector3f::cross(const Vector3f& vecIn)const  {
	return Vector3f(vecIn.y*z - vecIn.z*y, vecIn.z*x
		- vecIn.x*z, vecIn.x*y - vecIn.y*x);
}

inline Vector3f Vector3f::normalize()const {
	return Vector3f(*this) / length();
}

inline float Vector3f::length() const {
	return  sqrt(x*x + y*y + z*z);
}

using vec3f = Vector3f;
}  // namespace Angine
