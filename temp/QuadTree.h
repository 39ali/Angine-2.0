#pragma once
#include <vector>
#include "Particles.h"
#include "Cube.h"
#define MAX_PARTICLES 10
#define MAX_LEVELS 5

class QuadTree
{
public:
	QuadTree(int level, Cube bound);
	QuadTree() {};
	~QuadTree();
	void insert(Particle* particle);
	void clear();
	void retrieve( std::vector<Particle*>** list, Particle* p);

private:
	int getIndex(Particle* particle);
	void split();
	int m_level;
	std::vector<Particle*> m_particles;
	std::vector<QuadTree> m_nodes;
	Cube m_bound;
};