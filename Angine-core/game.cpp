#include "game.h"
#include <iostream>

Game::Game()
{
	createWindow(800, 600, "game");
};
Game::~Game() {};

std::vector<int> vec;
Entity* entity;
void Game::init()
{



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
	int indices[] = {
		0,1,3,
		3,1,2
	};


	std::string filename = "../Models/cube.obj";
	Model*	model1 = addModel(filename);
	glm::mat4 transform1 = glm::mat4(1.f);//glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f)); //* glm::scale(glm::mat4(), glm::vec3(0.3f, 0.3f, 0.3f));
	entity = new Entity(model1, transform1);
	AddEntity(entity);


};
void Game::update()
{
	entity->transform = glm::translate(glm::mat4(), glm::vec3(0.2f, 0, 0))*glm::scale(glm::mat4(), glm::vec3(0.5f,0.5f, 0.5f))* glm::rotate(glm::mat4(), (float)Time::getTime() * glm::radians(45.f), glm::vec3(1, 1, 1));
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