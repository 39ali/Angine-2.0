#include "game.h"

Game::Game() :m_particlesNum(glm::vec2(40, 40)), m_clothSize(glm::vec2(4, 3))
{
	createWindow(1024, 1280, "game");
};
Game::~Game()
{


};
glm::vec3 scale(0.35f, 0.35, 0.35);
glm::vec3 pos = glm::vec3(0, 2, -5);
glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0, 0, 0));

glm::vec3 lightPos(glm::vec3(0, 0, 10));


void Game::init()
{

	projection = glm::perspective(glm::radians(65.f), ((float)Window::getWidth() / (float)Window::getHeight()), 0.1f, 1000.0f);
	tps = new  FPSCamera(projection);
	setCamera(tps);
	m_Tex = TextureManager::LoadTexture("Textures/tex.jpg");

	m_cloth = new Cloth(m_particlesNum, m_clothSize, model, m_Tex);
	AddCloth(m_cloth);



	Material * material = new BasicLightingMaterial(new Shader("../Shaders/BasicShader.vs", "../Shaders/BasicShader.fs"), tps);
	material->setUniform("lightPos", lightPos);
	m_cloth->setLightPos(lightPos);
	

	//sphere
	sphere_ent = new Entity(Loader::loadModelFromFile("Models/SphereHighPoly.obj"), glm::translate(glm::mat4(), pos)* glm::scale(glm::mat4(), scale));
	sphere_ent->setMaterial(material);
	AddEntity(sphere_ent);


	///plane 
	Entity*	plane = new Entity(ModelFactory::createPlane(), glm::translate(glm::mat4(), glm::vec3(1, -1, 0))* glm::scale(glm::mat4(), glm::vec3(3)));
	plane->getModel()->m_meshes[0]->m_textures->push_back(TextureManager::LoadTexture("Textures/wood.jpg"));
	plane->getModel()->m_meshes[0]->hasTextures = true;
	plane->setMaterial(material);
	AddEntity(plane);

	/*"Textures/SkyBox/right.png",
		"Textures/SkyBox/left.png",
		"Textures/SkyBox/top.png",
		"Textures/SkyBox/bot.png",
		"Textures/SkyBox/back.png",
		"Textures/SkyBox/front.png"*/
		///skybox 
	std::vector < std::string > cubemaps =
	{
		"Textures/nightsky/nightsky_rt.tga",
		"Textures/nightsky/nightsky_lf.tga",
		"Textures/nightsky/nightsky_up.tga",
		"Textures/nightsky/nightsky_dn.tga",
		"Textures/nightsky/nightsky_bk.tga",
		"Textures/nightsky/nightsky_ft.tga",

	};
	AddSkyBox(new SkyBox(cubemaps));
};


void Game::onRender()/// use for special  stuff
{

}

void Game::update()
{
      pos = glm::vec3(2, 1.5, (float)glm::cos(Time::getTime() / 3)*-5);
	sphere_ent->transform = glm::translate(glm::mat4(), pos)* glm::scale(glm::mat4(), scale);
	
	if (Window::isKeyPressed(GLFW_KEY_Q)) {
		pos.z += 1 * Time::getDeltaTime();
		sphere_ent->transform = glm::translate(glm::mat4(), pos)* glm::scale(glm::mat4(), scale);
	}
	else if (Window::isKeyPressed(GLFW_KEY_Z))
	{
		pos.z -= 1 * Time::getDeltaTime();
		sphere_ent->transform = glm::translate(glm::mat4(), pos)* glm::scale(glm::mat4(), scale);
	}
	m_cloth->updateSphere(pos, 1.0f);
	

	if (Window::isKeyPressed(GLFW_KEY_C))
	{
		Window::getInstance()->disableCursor(0);
	}
	else if (Window::isKeyPressed(GLFW_KEY_V))
	{
		Window::getInstance()->disableCursor(1);
	}

};
void Game::tick()
{
	Scene::printFpsUps();
};


int main()
{
	Game game;

	game.start();
	return 0;
}