#include "Material.h"

namespace Angine
{
	namespace Renderer
	{

		Material::Material(Shader* shader) :m_shader(shader)
		{
			
		}

		Material::~Material()
		{

			delete m_shader;
		}


	}

}