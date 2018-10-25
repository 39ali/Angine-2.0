#pragma once
#include "Shader.h"
namespace Angine {
	namespace Renderer {
		class Material
		{
		public:
			Material(Shader* shader);
			~Material();
			void setSpecular();
			void setDiffuse();
			template<typename T>
			void setUniform(const std::string& name, const T& value)
			{
				//m_shader->use();
				//m_shader->setUniform(name.c_str(), value);
			}

			void use() {};
				void unuse() {};             
			virtual void  onRender() {};
		protected:
			Shader*	m_shader;


		};

	}
}