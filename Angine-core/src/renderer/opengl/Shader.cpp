#include "Shader.h"
#include <vector>

Shader::Shader(const char * vertexshaderloc, const char* fragmentshaderloc)
{
	m_program_id = glCreateProgram();

	m_vs_id = compile(vertexshaderloc, GL_VERTEX_SHADER);
	m_fg_id = compile(fragmentshaderloc, GL_FRAGMENT_SHADER);
	link();
};


GLuint Shader::compile(const char * shaderloc, GLenum type)
{

	std::string shaderSource = Utils::read_file(shaderloc);

	GLuint shader = glCreateShader(type);

	const GLchar *source = (const GLchar *)shaderSource.c_str();
	glShaderSource(shader, 1, &source, 0);


	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

		std::cout << "shader compile  error : " << shaderloc << " : " << std::string(infoLog.begin(), infoLog.end()) << std::endl;

		glDeleteShader(shader);


		return 0;
	}
	glAttachShader(m_program_id, shader);
	return shader;
};

void  Shader::link() const
{
	glLinkProgram(m_program_id);

	GLint isLinked = 0;
	glGetProgramiv(m_program_id, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_program_id, maxLength, &maxLength, &infoLog[0]);

		std::cout << "shader linking error : " << std::string(infoLog.begin(), infoLog.end()) << std::endl;

		glDeleteProgram(m_program_id);

		glDeleteShader(m_vs_id);
		glDeleteShader(m_vs_id);

		return;
	}

	glDetachShader(m_program_id, m_vs_id);
	glDetachShader(m_program_id, m_fg_id);
}


void Shader::use()const
{
	glUseProgram(m_program_id);
}

void Shader::unuse()const
{
	glUseProgram(0);
}

Shader::~Shader()
{
	glDeleteProgram(m_program_id);
	glDeleteShader(m_vs_id);
	glDeleteShader(m_vs_id);
};


void Shader::setUniform(const char* name, glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_program_id, name), 1, GL_FALSE, &value[0][0]);
}
void Shader::setUniform(const char * name, const glm::vec3& value) const
{
	glUniform3f(glGetUniformLocation(m_program_id, name), value.x, value.y, value.z);
}
void Shader::setUniform(const char * name, const float& value) const
{
	glUniform1f(glGetUniformLocation(m_program_id, name), value);
}

void Shader::setUniform(const char * name, const int& value) const
{
	glUniform1i(glGetUniformLocation(m_program_id, name), value);
};