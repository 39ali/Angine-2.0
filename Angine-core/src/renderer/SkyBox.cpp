#include "SkyBox.h"	


namespace Angine {
	namespace Renderer {

		SkyBox::SkyBox(const std::vector<std::string>& faces)
		{
			m_tex = TextureManager::LoadCubeMap(faces);
			//m_Shader = new Shader("../Shaders/SkyBox/SkyBox.vs", "../Shaders/SkyBox/SkyBox.fs");
			//m_Shader->use();
			//m_Shader->setUniform("skybox", 0);
			initBuffer();
		}

		void SkyBox::initBuffer()
		{
			float skyboxVertices[] = {
				// positions          
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				1.0f, -1.0f,  1.0f
			};
			glGenBuffers(1, &m_vbo);
			glGenVertexArrays(1, &m_vio);

			glBindVertexArray(m_vio);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);


		}
		void SkyBox::Draw(Camera* cam)
		{
			glDepthFunc(GL_LEQUAL);
			//m_Shader->use();
			//m_Shader->setUniform("projection", cam->getPorjection());
			//m_Shader->setUniform("view", glm::mat4(glm::mat3(cam->getMatrix())));
			glBindVertexArray(m_vio);
			glActiveTexture(GL_TEXTURE0);
			m_tex->bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthFunc(GL_LESS);
		}

		SkyBox::~SkyBox()
		{
			delete m_Shader;
		}

	}
}