#pragma once
#include <glm\glm.hpp>
#include "../common.h"
class Color {
public :
	Color(const glm::vec4& color):data(color) {
		
	};
	inline  float operator[](uint32 index)const;
private:
	glm::vec4  data;

};




inline  float Color::operator[](uint32 index)const {
	return data[index];
}