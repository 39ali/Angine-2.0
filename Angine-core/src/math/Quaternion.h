#pragma once

#include "Vector3f.h"

namespace Angine {	
	class Quaternion {
public :
	Quaternion() { x = y = z = w = 0.0f; }
  Quaternion(float _x, float _y, float _z, float w);
  Quaternion normalize()const ;
  Quaternion conjugate()const ;
  vec3f quatToDegrees()const;
  static vec3f rotate(float angle, const vec3f& axe, const vec3f& v);
  float x, y, z, w;
};

	Quaternion operator*(const Quaternion& q1, const Quaternion& q2);

	Quaternion operator*(const Quaternion& q, const vec3f& v);
}  // namespace Angine
