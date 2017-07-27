#pragma once
#include "../common.h"
#include <FreeImage\FreeImage.h>
#include "../Utils/imageLoader.h"

namespace Angine {
	namespace Renderer
	{

		class Texture2D
		{
		public:
			Texture2D(const char* fileName);
			~Texture2D();
			void bind() const;
			void unbind() const;
		private:
			GLuint m_texID;
			unsigned int m_width, m_height;
			const char * fileName;
			bool LoadTexture(const char* filename);
		};
	}
}