#pragma once
#include "Shader.h"
namespace Angine {
	namespace Renderer {
		class Material
		{
		public:
			Material(Shader* shader) ;
			~Material();
			void setSpecular();
			void setDiffuse();


		private:
			Shader*	m_shader;


		};

	}
}