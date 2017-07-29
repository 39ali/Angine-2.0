#pragma once
#include "../renderer/Mesh.h"
#include "../common.h"
#include <unordered_map>
#include "../renderer/Model.h"

using namespace Angine::Renderer;
namespace Angine {
	namespace Core {

		class Entity
		{
		public:
			Entity(Model* model, glm::mat4& transofrm = glm::mat4(0));
			~Entity();

			glm::mat4 transform;
			inline  Model* getModel() const { return m_Model; };

		private:
			Model* m_Model;
			//	std::unordered_map<ComponentType*, Component*>();

		};


	}
}