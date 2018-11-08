#pragma once
#include "../math/Vector2f.h"
#include "../math/Vector3f.h"
namespace Angine
{
		struct VertexData
		{

			VertexData() {};
			VertexData(const vec3f& _pos, const vec2f& _uv, const vec3f& _normal) :position(_pos), uv(_uv), normal(_normal) {};
			VertexData(const vec3f& _pos, const vec2f& _uv) :position(_pos), uv(_uv) {};
			vec3f position;
			vec2f uv;
			vec3f normal;

		};
}