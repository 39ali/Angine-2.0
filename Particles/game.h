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
	void initbuffers();
	glm::vec2 getNormalizedDeviceCoords(double& x, double& y);
private:
	Shader * m_shader ,* m_computeShader;
	glm::ivec3 m_nParticles;
	glm::vec4 bh1, bh2;
	unsigned int particlesSum;
	GLuint m_Pvao, m_bhBuf, m_bhVao;
	int m_width, m_height;
	glm::mat4 m_inverse;
};