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
	glm::vec2 m_particlesNum, m_clothSize;

	Camera*	tps;
	Entity* sphere_ent;
	std::vector<glm::vec4> m_positions;
	Texture2D* m_Tex;
	Cloth* m_cloth;
};