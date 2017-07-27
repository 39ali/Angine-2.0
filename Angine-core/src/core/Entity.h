#pragma once
#include "../renderer/RawModel.h"
#include "../common.h"
#include <unordered_map>

using namespace Angine::Renderer;
namespace Angine {
	namespace Core {

		class Entity
		{
		public:
			Entity(RawModel* model, glm::mat4& transofrm = glm::mat4(0));
			~Entity();

			glm::mat4 transform;
			inline const RawModel* getRawModel() const { return m_rawModel; };

		private:
			RawModel* m_rawModel;

		};


	}
}