#include "game.h"

Game::Game()
{//1024x1280 
	createWindow(1024, 1280, "game");
};
Game::~Game() {};

std::vector<int> vec;
Entity* entity;
void Game::init()
{

	std::vector<VertexData> v;
	float uv[] =
	{ 0,0,
		0,1,
		1,1,
		1,0
	};

	float vertices[] = {
		-0.5f, 0.5f,0.0f,
		-0.5f, -0.5f,0.0f,
		0.5f, -0.5f,0.0f,
		0.5f, 0.5f,0.0f
	};
	std::vector<unsigned int > indices = {
		0,1,3,
		3,1,2
	};
	for (int i = 0; i < sizeof(vertices) / 4; i += 3)
	{
		VertexData ver;
		ver.position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
		v.push_back(ver);
	}

	for (int i = 0; i < v.size(); i++)
	{
		v[i].uv = glm::vec2(uv[2 * i + 0], uv[2 * i + 1]);
	}


	//"../Models/nanosuit/nanosuit.obj";
	//std::string filename = "../Models/nanosuit/nanosuit.obj"; ///"../Models/nanosuit/nanosuit.obj";
	//Model*	model1 = Loader::loadModelFromFile(filename);
	//Model* cube = Loader::loadModelFromFile("../Models/cube.obj");
	////	Model* rect = Loader::loadModelFromVertices((const std::vector<VertexData>)v, (const std::vector<unsigned int >)indices);
	//glm::mat4 transform1 = glm::mat4(1.f);//glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f)); //* glm::scale(glm::mat4(), glm::vec3(0.3f, 0.3f, 0.3f));
	//entity = new Entity(model1, transform1);
	//glm::mat4 transform2 = glm::translate(glm::mat4(), glm::vec3(5, 2, 0)) * glm::scale(glm::mat4(), glm::vec3(1, 1, 1));
	////	Entity* entity1 = new Entity(rect, transform2);
	//	Entity* entity1 = new Entity(cube, transform2);
	//AddEntity(entity);
	//AddEntity(entity1);

};
void Game::update()
{
	//entity->transform = glm::translate(glm::mat4(), glm::vec3(0.f, -2.0f, 0))*glm::scale(glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f))* glm::rotate(glm::mat4(), (float)Time::getTime() * glm::radians(45.f), glm::vec3(0, 1, 0));
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