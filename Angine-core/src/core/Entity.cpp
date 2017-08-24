#include  "Entity.h"

namespace Angine {
	namespace Core {

		Entity::Entity(Model* model, glm::mat4& Transofrm ) : m_mode(RenderMode::Fill)
		{
			transform = Transofrm;
			m_Model = model;
		}
		Entity::~Entity() {};




	}
}