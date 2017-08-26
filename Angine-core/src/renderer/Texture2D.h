#pragma once
#include "../common.h"
#include <FreeImage\FreeImage.h>
#include "../Utils/imageLoader.h"

namespace Angine {
	namespace Renderer
	{

		enum TextureType
		{
			Diffuse, Specular
		};

		class Texture2D
		{
		public:
			Texture2D() {};
			~Texture2D() {};
			void bind() const;
			void unbind() const;
			GLuint m_texID;
			unsigned int m_width, m_height;
			std::string fileName;
			TextureType m_type;
			
		};
	}
}