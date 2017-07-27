#include "Texture2D.h"


namespace Angine {
	namespace Renderer
	{

		Texture2D::Texture2D(const char* fileName)
		{
			LoadTexture(fileName);
		}
		Texture2D::~Texture2D()
		{


		}


		bool Texture2D::LoadTexture(const char* filename)
		{
			BYTE* data = Utils::imageLoader(filename, m_width, m_height);
			glGenTextures(1, &m_texID);
			glBindTexture(GL_TEXTURE_2D, m_texID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGRA , GL_UNSIGNED_BYTE, data);
			return true;
			//delete[] data;
		}

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