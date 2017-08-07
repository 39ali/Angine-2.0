#pragma once
#include <Angine.h>

class Game : public Angine::Core::Scene
{
public:
	Game();
	~Game();
private:
	void init() override;
	void update() override;
	void tick() override;
	void onRender() override;


private:
	void initBuffers();
private:
	glm::vec2 m_particlesNum, m_clothSize;
	GLuint m_pos[2], m_vel[2], m_normal, m_texCoords, m_indices, m_vao;
	unsigned int m_elementNum;
	Shader* m_shader, *m_computeCloth, *m_computeNormals;
	GLuint swap=0;
};