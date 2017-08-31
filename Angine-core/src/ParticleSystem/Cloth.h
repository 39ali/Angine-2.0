#pragma once
#include "../common.h"
#include "../renderer/Shader.h"
#include "../renderer/Texture2D.h"
#include "../renderer/Camera.h"
#define PRIMITIVE_RESTART 0xffffff
using namespace Angine::Renderer;
namespace Angine
{
	namespace ParticleSystem {
		class Cloth
		{
		public:
			Cloth(const glm::vec2 &particlesNum, const glm::vec2 &clothSize, const glm::mat4  &model, Texture2D * tex);
			~Cloth();
			void render(Camera* cam);
			void updateSphere(const glm::vec3& pos, float radius);
			void mousePicker(Camera * cam);
		
			void setLightPos(const glm::vec3& pos) {
				m_shader->use();
				m_shader->setUniform("lightPos", pos);
				m_shader->unuse();
			}
		private:
			glm::vec2 m_particlesNum, m_clothSize;
			GLuint m_pos[2], m_vel[2], m_normal, m_texCoords, m_indices, m_vao , m_spring[2];
			unsigned int m_elementNum;
			Shader* m_shader, *m_computeCloth, *m_computeNormals;
			GLuint swap = 0;
			glm::mat4 m_clothModel;
			Texture2D* m_Tex;
			std::vector<glm::vec4> m_positions;
			glm::mat4 m_model = glm::translate(glm::mat4(), glm::vec3(0, 0, 0));
		private:
			void initBuffers();
		};
	}
}