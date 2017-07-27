#include  "Entity.h"

namespace Angine {
	namespace Core {

		Entity::Entity(RawModel* model, glm::mat4& Transofrm )
		{
			transform = Transofrm;
			m_rawModel = model;
		}
		Entity::~Entity() {};




	}
}