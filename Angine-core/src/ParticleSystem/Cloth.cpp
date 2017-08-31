#include "Cloth.h"


namespace Angine
{
	namespace ParticleSystem {


		Cloth::Cloth(const glm::vec2 &particlesNum, const glm::vec2 &clothSize, const glm::mat4 &model, Texture2D * tex) :
			m_particlesNum(particlesNum), m_clothSize(clothSize), m_model(model), m_Tex(tex)

		{
			glEnable(GL_PRIMITIVE_RESTART);
			glPrimitiveRestartIndex(PRIMITIVE_RESTART);
			m_shader = new Shader("../Shaders/Cloth/cloth.vs", "../Shaders/Cloth/cloth.fs");
			m_computeCloth = new Shader("../Shaders/Cloth/cloth.cs");
			m_computeNormals = new Shader("../Shaders/Cloth/normal.cs");

			m_computeCloth->use();
			float dx = m_clothSize.x / (m_particlesNum.x - 1);
			float dy = m_clothSize.y / (m_particlesNum.y - 1);
			m_computeCloth->setUniform("RestLengthHoriz", dx);
			m_computeCloth->setUniform("RestLengthVert", dy);
			m_computeCloth->setUniform("RestLengthDiag", sqrtf(dx * dx + dy * dy));


			initBuffers();
		}


		void Cloth::initBuffers()
		{
			std::vector<glm::vec4> vel(m_particlesNum.x*m_particlesNum.y, glm::vec4(0));
			std::vector<glm::vec2> texCoords;
			std::vector<unsigned int>indices;
			float dx = m_clothSize.x / (m_particlesNum.x - 1);
			float dy = m_clothSize.y / (m_particlesNum.y - 1);

			float ds = 1.0f / (m_particlesNum.x - 1);
			float dt = 1.0f / (m_particlesNum.y - 1);

			glm::vec4 p(0.0f, 0.0f, 0.0f, 1.0f);
			glm::vec2 texc;
			for (int y = 0; y < m_particlesNum.y; y++)
			{
				for (int x = 0; x < m_particlesNum.x; x++)
				{
					p.x = dx*x;
					p.y = dy*y;
					m_positions.push_back(m_model*p);

					texc.x = x*ds;
					texc.y = dt*y;
					texCoords.push_back(texc);
				}
			}

			//indices
			for (int y = 0; y < m_particlesNum.y - 1; y++)
			{
				for (int x = 0; x < m_particlesNum.x; x++)
				{
					indices.push_back((y + 0)*m_particlesNum.x + x);
					indices.push_back((y + 1)*m_particlesNum.x + x);
				}
				indices.push_back(PRIMITIVE_RESTART);
			}
			m_elementNum = indices.size();


			GLuint bufs[9];
			glGenBuffers(9, bufs);
			m_pos[0] = bufs[0];
			m_pos[1] = bufs[1];
			m_vel[0] = bufs[2];
			m_vel[1] = bufs[3];
			m_normal = bufs[4];
			m_indices = bufs[5];
			m_texCoords = bufs[6];
			m_spring[0] = bufs[7];
			m_spring[1] = bufs[8];



			unsigned int size = sizeof(glm::vec4)*m_positions.size();

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_pos[0]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, m_positions.data(), GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_pos[1]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, m_positions.data(), GL_DYNAMIC_DRAW);


			//vel
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_vel[0]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_COPY);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_vel[1]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_COPY);

			//normal 
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_normal);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_COPY);

			//indices
			glBindBuffer(GL_ARRAY_BUFFER, m_indices);
			glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_COPY);


			// Texcoord
			glBindBuffer(GL_ARRAY_BUFFER, m_texCoords);
			glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);

			// spring up
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_spring[0]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int)* m_positions.size(), NULL, GL_DYNAMIC_COPY);
			glBindBuffer(GL_ARRAY_BUFFER, m_spring[0]);
			glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(3);

			// spring down
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_spring[1]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int)* m_positions.size(), NULL, GL_DYNAMIC_COPY);
			glBindBuffer(GL_ARRAY_BUFFER, m_spring[1]);
			glVertexAttribPointer(4, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(4);



			glGenVertexArrays(1, &m_vao);
			glBindVertexArray(m_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_pos[0]);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, m_normal);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, m_texCoords);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
			glBindVertexArray(0);
		}


		void Cloth::render(Camera* cam)
		{

			m_computeCloth->use();
			for (int i = 0; i < 2000; i++)
			{
				glDispatchCompute(m_particlesNum.x / 10, m_particlesNum.y / 10, 1);//how many work groups
				glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

				swap = 1 - swap;
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_pos[swap]);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_pos[1 - swap]);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_vel[swap]);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_vel[1 - swap]);
			}

			m_computeNormals->use();
			glDispatchCompute(m_particlesNum.x / 10, m_particlesNum.y / 10, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

			m_shader->use();
			glActiveTexture(GL_TEXTURE0);
			m_Tex->bind();
			m_shader->setUniform("view", cam->getMatrix());
			m_shader->setUniform("model", glm::mat4(1));
			m_shader->setUniform("projection", cam->getPorjection());

			m_shader->setUniform("diffuseMap", 0);
			m_shader->setUniform("cameraPos", cam->getPos());



			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLE_STRIP, m_elementNum, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);


		}


		void Cloth::updateSphere(const glm::vec3& pos, float radius)
		{
			m_computeCloth->use();
			m_computeCloth->setUniform("SphereR", radius);
			m_computeCloth->setUniform("SpherePos", pos);

		}

		int selected = -1;
		void Cloth::mousePicker(Camera * cam)
		{

			if (Window::isMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {


				if (selected != -1)
				{
					const glm::vec3 & right = cam->getRight();
					m_computeCloth->use();
					m_computeCloth->setUniform("selectedVert", selected);
					glBindBuffer(GL_ARRAY_BUFFER, m_pos[0]);
					glm::vec4* ptr = (glm::vec4*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
					glm::vec3 force = glm::vec3(ptr[selected]) + glm::vec3(-right[0] * Window::getDx(), Window::getDy()
						, right[2] * Window::getDx()) * 10.f;
					m_computeCloth->setUniform("mouseForce", force);
					glUnmapBuffer(GL_ARRAY_BUFFER);
					return;
				}

				float winZ;
				double x, y;
				Window::getMousePos(x, y);
				glReadPixels(x, Window::getHeight() - y, 1, 1, GL_DEPTH_COMPONENT,
					GL_FLOAT, &winZ);

				if (winZ == 1)
					winZ = 0;

				glm::vec3 objPt = glm::unProject(glm::vec3
					(x, Window::getHeight() - y, winZ), glm::mat4(1)*cam->getMatrix(), cam->getPorjection(), glm::vec4(0, 0, Window::getWidth(), Window::getHeight()));

				size_t i = 0;
				float minDist = 1000.0f;
				glBindBuffer(GL_ARRAY_BUFFER, m_pos[0]);

				for (i = 0; i < m_positions.size(); i++)
				{
					glm::vec4* ptr = (glm::vec4*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
					// if the pointer is valid(mapped), update VBO
					if (ptr) {
						float dist = glm::distance(glm::vec3(ptr[i]), objPt);
						if (dist < 0.7 && dist < minDist) {
							selected = i;
							minDist = dist;
						}
						glUnmapBuffer(GL_ARRAY_BUFFER);
					}
				}

				if (selected != -1) {
					std::cout << selected << std::endl;
				}
			}
			else
			{
				selected = -1;
				m_computeCloth->use();
				m_computeCloth->setUniform("selectedVert", selected);
			}
		}

		



		Cloth::~Cloth()
		{

			delete m_shader, m_computeCloth, m_computeNormals;

		}


	}
}