#pragma once
#include <vector>
#include "GameObject.h"
class Scene
{
public:
	Scene();
	~Scene();
	void Init();
	std::vector<GameObject*> m_gameObjects;
};