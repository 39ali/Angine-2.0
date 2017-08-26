#pragma once
#include "Texture2D.h"
#include <unordered_map>
#include "../common.h"
namespace Angine {
	namespace Renderer {
		class TextureManager
		{

		public:
			static	Texture2D* LoadTexture(const char* filename);
			static	Texture2D* LoadCubeMap(const std::vector<std::string>& faces);
			static void Clean();
		private:
			TextureManager() {};
			~TextureManager();
			static	std::unordered_map < std::string, Texture2D*> m_textures;

		};
	}
}