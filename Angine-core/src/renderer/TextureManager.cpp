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
			FIBITMAP * dib(0);
			BYTE*data = imageLoader(filename, texture->m_width, texture->m_height, BPP, dib);
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
			free( data);
			
			return texture;
			//delete[] data;  // TODO: figure out how to delete this  .
		}

		Texture2D* TextureManager::LoadCubeMap(const std::vector<std::string>& faces)
		{
		
			Texture2D* texture = new Texture2D();
			glGenTextures(1, &texture->m_texID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_texID);

			unsigned int  BPP;
			FIBITMAP * dib(0);
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				BYTE* data = imageLoader(faces[i].c_str(), texture->m_width, texture->m_height, BPP, dib);
				if (data)
				{
					if (BPP == 24) {
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texture->m_width, texture->m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
					}
					else if (BPP == 32)
					{
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texture->m_width, texture->m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
					}
				}
				else
				{
					std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
					
				}
				free( data);
				
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			m_textures.insert({ faces[0], texture });
			return texture;

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