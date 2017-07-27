#include "game.h"
#include <iostream>
#include "src\Utils\Tiny_obj_loader.h"
Game::Game()
{
	createWindow(800, 600, "game");
};
Game::~Game() {};

std::vector<int> vec;
Entity* entity;
void Game::init()
{



	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	std::string filename = "../Models/cube.obj";
	std::string material = "../Models/";
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), material.c_str(), true);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}







	float uv[] =
	{ 0,0,
	 0,1,
	1,1,
	1,0
	};

	float vertices[] = {
		-0.5f, 0.5f,0,
		-0.5f, -0.5f,0,
		0.5f, -0.5f,0,
		0.5f, 0.5f,0
	};
	int indices[] = {
		0,1,3,
		3,1,2
	};
	RawModel*	model = addRawModel(vertices, sizeof(vertices), indices, sizeof(indices), uv, sizeof(uv));
	glm::mat4 transform = glm::translate(glm::mat4(), glm::vec3(0.2f, 0, 0))* glm::rotate(glm::mat4(), (float)Time::getTime() * glm::radians(45.f), glm::vec3(0, 0, 1));

	entity = new Entity(model, transform);
	AddEntity(entity);
};
void Game::update()
{

	entity->transform = glm::translate(glm::mat4(), glm::vec3(0.2f, 0, 0))* glm::rotate(glm::mat4(), (float)Time::getTime() * glm::radians(45.f), glm::vec3(0, 0, 1));
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