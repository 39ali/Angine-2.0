#include "QuadTree.h"



QuadTree::QuadTree(int level, Cube bound) :m_bound(bound), m_level(level)
{
	m_nodes.reserve(8);
}
void QuadTree::insert(Particle* particle)
{
	if (!m_nodes.empty())
	{
		int index = getIndex(particle);

		if (index != -1) {
			m_nodes[index].insert(particle);
			return;
		}
	}

	m_particles.push_back(particle);

	if (m_particles.size() > MAX_PARTICLES && m_level < MAX_LEVELS)
	{
		if (m_nodes.empty())
		split();
		
		int i = 0;
		while (i < m_particles.size())
		{
			int index = getIndex(m_particles[i]);
			if (index != -1)
			{
				m_nodes[index].insert(m_particles[i]);
				m_particles.erase(m_particles.begin() + i);
			}
			else { i++; }

		}
	}

}


void QuadTree::retrieve( std::vector<Particle*>** list, Particle* p)
{
	int index = getIndex(p);
	if (index != -1 && !m_nodes.empty()) {
		m_nodes[index].retrieve(list, p);
		return;
	}

	*list =&m_particles;
}

void QuadTree::clear()
{
	m_particles.clear();
	for (int i = 0; i < m_nodes.size(); i++)
	{
		m_nodes[i].clear();
	}
}

void QuadTree::split()
{
	glm::vec3 subCenter = m_bound.m_center / 2.0f;
	float subrad = m_bound.m_radius / 2.0f;
	

	//top left
	m_nodes.push_back(QuadTree(m_level + 1, Cube(glm::vec3(m_bound.m_center.x - subrad, m_bound.m_center.y + subrad, m_bound.m_center.z - subrad), subrad)));
	m_nodes.push_back(QuadTree(m_level + 1, Cube(glm::vec3(m_bound.m_center.x - subrad, m_bound.m_center.y + subrad, m_bound.m_center.z + subrad), subrad)));
	//buttom left
	m_nodes.push_back(QuadTree(m_level + 1, Cube(glm::vec3(m_bound.m_center.x - subrad, m_bound.m_center.y - subrad, m_bound.m_center.z - subrad), subrad)));
	m_nodes.push_back(QuadTree(m_level + 1, Cube(glm::vec3(m_bound.m_center.x - subrad, m_bound.m_center.y - subrad, m_bound.m_center.z + subrad), subrad)));

	//top right
	m_nodes.push_back(QuadTree(m_level + 1, Cube(glm::vec3(m_bound.m_center.x + subrad, m_bound.m_center.y + subrad, m_bound.m_center.z - subrad), subrad)));
	m_nodes.push_back(QuadTree(m_level + 1, Cube(glm::vec3(m_bound.m_center.x + subrad, m_bound.m_center.y + subrad, m_bound.m_center.z + subrad), subrad)));

	//buttom right
	m_nodes.push_back(QuadTree(m_level + 1, Cube(glm::vec3(m_bound.m_center.x + subrad, m_bound.m_center.y - subrad, m_bound.m_center.z - subrad), subrad)));
	m_nodes.push_back(QuadTree(m_level + 1, Cube(glm::vec3(m_bound.m_center.x + subrad, m_bound.m_center.y - subrad, m_bound.m_center.z + subrad), subrad)));
}

int QuadTree::getIndex(Particle* particle)
{
	const glm::vec3& pos = particle->getPosition();

	if (m_nodes.empty()) 
	{
		return -1;
	}

	for (int i = 0; i < 8; i++) {
		if (pos.x > m_nodes[i].m_bound.m_center.x + m_nodes[i].m_bound.m_radius ||
			pos.x < m_nodes[i].m_bound.m_center.x - m_nodes[i].m_bound.m_radius) {
			continue;
		}

		if (pos.y > m_nodes[i].m_bound.m_center.y + m_nodes[i].m_bound.m_radius ||
			pos.y < m_nodes[i].m_bound.m_center.y - m_nodes[i].m_bound.m_radius) {
			continue;
		}

		if (pos.z > m_nodes[i].m_bound.m_center.z + m_nodes[i].m_bound.m_radius ||
			pos.z < m_nodes[i].m_bound.m_center.z - m_nodes[i].m_bound.m_radius) {
			continue;
		}
		return i;
	}
	return -1;

}



QuadTree::~QuadTree()
{
}