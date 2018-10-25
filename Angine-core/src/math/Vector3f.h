#pragma once
namespace Angine {
class Vector3f {
 public:
  Vector3f();
  Vector3f(float val);
  Vector3f(float _x, float _y, float _z);
  Vector3f(const Vector3f &vecIn);
  inline float dot(const Vector3f &vecIn);
  inline Vector3f cross(const Vector3f &vecIn);
  Vector3f operator+(const Vector3f &vec3);
  Vector3f operator+=(const Vector3f &vec3);

  Vector3f operator-(const Vector3f &vec3);
  Vector3f operator-=(const Vector3f &vec3);

  Vector3f operator*(const Vector3f &vec3);
  Vector3f operator*(const float value);
  Vector3f operator*=(const Vector3f &vec3);
  Vector3f operator*=(const float value);
  union {
	  struct { float x, y, z; };
    float v[3];
  };
};

using vec3f = Vector3f;

}  // namespace Angine
