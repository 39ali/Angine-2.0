#pragma once
#include "Vector3f.h"
#include "../common.h"
namespace Angine {
	class Color {
	public:
		Color(const vec3f& color, float alpha) :data(color), alpha(alpha) {

		};
		inline  float operator[](uint32 index)const;
	private:
		vec3f  data;
		float alpha;
	};




	inline  float Color::operator[](uint32 index)const {
		return data[index];
	}
}