#pragma once
#include <Angine.h>
class Cube
{
public:
	Cube() {};
	Cube(const glm::vec3& center, float radius) :m_center(center), m_radius(radius){};
	glm::vec3 m_center;
	float  m_radius;
};