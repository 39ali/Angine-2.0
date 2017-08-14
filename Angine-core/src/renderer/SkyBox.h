#pragma once
#include "../common.h"
#include "../renderer/TextureManager.h"
#include "../renderer/Shader.h"
#include "Camera.h"
namespace Angine {
	namespace Renderer {
		class SkyBox
		{
		public:
			SkyBox(const std::vector<std::string>& maps);
			void Draw( Camera* cam);
			~SkyBox();
		private:
			void initBuffer();

		private:
			Texture2D* m_tex;
			GLuint m_vbo, m_vio;
			Shader * m_Shader;
		};

	}
}