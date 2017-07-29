#include "Texture2D.h"


namespace Angine {
	namespace Renderer
	{

		void Texture2D::bind() const
		{
			glBindTexture(GL_TEXTURE_2D, m_texID);
		}
		void Texture2D::unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}



}