#pragma once
#define GLEW_STATIC
#include <glew\glew.h>
#include <glm\glm.hpp>
#include <iostream>
#include "../../Utils/Reader.h"

class Shader
{
public:
	Shader(const char * vertex_shader_loc, const char* fragment_shader_loc);
	~Shader();

public:
	void use()const;
	void unuse()const;
	inline GLuint& getProgramId() { return m_program_id; };
public:

	void setUniform(const char* name, glm::mat4& value) const;
	void setUniform(const char * name, const glm::vec3& value) const;
	void setUniform(const char * name, const float& value) const;
	void setUniform(const char * name, const int& value) const;
private:
	GLuint compile(const char * shaderloc, GLenum type);
	void link() const;

private:
	GLuint m_program_id, m_vs_id, m_fg_id;
};

