#include "TextureManager.h"
namespace Angine {
	namespace Renderer
	{

		std::unordered_map < std::string, Texture2D*> TextureManager::m_textures;

		Texture2D * TextureManager::LoadTexture(const char* filename)
		{
			auto it = m_textures.find(filename);
			if (it != m_textures.end())
				return it->second;

			Texture2D* texture = new Texture2D();
			texture->fileName = filename;
			unsigned int BPP;
			BYTE* data = Utils::imageLoader(filename, texture->m_width, texture->m_height, BPP);
			glGenTextures(1, &texture->m_texID);
			glBindTexture(GL_TEXTURE_2D, texture->m_texID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			if (BPP == 24) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->m_width, texture->m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
			}
			else if (BPP == 32)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->m_width, texture->m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
			}
			m_textures.insert({ filename, texture });
			return texture;
			//delete[] data;  // TODO: figure out how to delete this  .
		}

		void TextureManager::Clean()
		{
			for each (auto texture in m_textures)
			{
				glDeleteTextures(1, &texture.second->m_texID);
				delete texture.second;
			}

			m_textures.clear();
		};

	}
}