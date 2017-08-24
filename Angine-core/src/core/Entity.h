#pragma once
#include "../renderer/Mesh.h"
#include "../common.h"
#include <unordered_map>
#include "../renderer/Model.h"

using namespace Angine::Renderer;
namespace Angine {
	namespace Core {

		enum RenderMode
		{
			Fill, Line
		};


		class Entity
		{
		public:
			Entity(Model* model, glm::mat4& transofrm = glm::mat4(0));
			~Entity();

			glm::mat4 transform;
			inline  Model* getModel() const { return m_Model; };
			void setMaterial(Material* material) { m_material = material; };
			inline  Material* getMaterial() { return m_material; }
			void setMode(RenderMode mode) { m_mode = mode; }
			inline RenderMode getMode() { return m_mode; }
			inline glm::vec3 getPosition() const { return glm::vec3(transform[3]); }
			void setPosition(const glm::vec3& pos) {
				transform[3].x = pos.x;  transform[3].y = pos.y;
				transform[3].y = pos.y;
			}
		private:
			Model* m_Model;
			Material * m_material;
			RenderMode m_mode;
			//	std::unordered_map<ComponentType*, Component*>();

		};


	}
}