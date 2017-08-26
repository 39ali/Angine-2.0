#include  "Entity.h"

namespace Angine {
	namespace Core {
		int Entity::m_id = 0;
		Entity::Entity(Model* model, glm::mat4& Transofrm) : m_mode(RenderMode::Fill)
		{
			id = m_id;
			m_id++;
			transform = Transofrm;
			m_Model = model;
		}
		Entity::~Entity() {};




	}
}