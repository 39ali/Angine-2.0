#include "Scene.h"

namespace Angine {
	namespace Core {

		Scene::Scene()
		{

		}
		Scene::~Scene()
		{
			delete m_renderer;

			delete m_particleShader;
			TextureManager::Clean();
		}

		glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.0f, 0)) * glm::rotate(glm::mat4(), glm::radians(0.f), glm::vec3(1, 1, 1))*glm::scale(glm::mat4(), glm::vec3(0.2, 0.2, 0.2));

		void Scene::render()
		{
			for each (auto entity in m_Entities)
			{

				Material * mat = entity->getMaterial();
				mat->use();
				mat->setUniform("projectionView", tps.getMatrix());
				mat->setUniform("model", model);
				mat->setUniform("cameraPos", tps.getPos());
				mat->setUniform("lightPos", glm::vec3(5, 2, 0));
				for each (Mesh*  mesh in entity->getModel()->m_meshes)
				{
					if (!mesh->hasTextures)
					{
						m_renderer->render(mesh);
						continue;
					}
					for (int i = 0; i < mesh->m_textures->size(); i++)
					{
						glActiveTexture(GL_TEXTURE0 + i);
						const Texture2D* tex = mesh->m_textures->at(i);
						tex->bind();
						const TextureType textype = tex->m_type;
						if (textype == TextureType::Diffuse)
						{
							mat->setUniform<int>("diffuseMap", i);
						}
						else if (textype == TextureType::Specular)
						{
							mat->setUniform<int>("specularMap", i);
						}
					}


					m_renderer->render(mesh);
				}
				mat->unuse();
			}


			//			m_staticShader->unuse();

		}
		void Scene::tick()
		{

		}

		void Scene::run()
		{
			double initialTime = Time::getTime();
			double updateTime = 1. / 60.;
			double updateTimer = 0.0;

			while (!m_window->isClosed())
			{
				m_window->clear();
				render();
				onRender();
				if (Time::getTime() - initialTime >= 1) // 1 sec has passed
				{
					tick();
					initialTime = Time::getTime();
					m_fps = 0.f;
					m_ups = 0.f;
				}

				if (Time::getTime() - updateTimer >= updateTime)
				{
					updateTimer += updateTime;
					internalUpdate();
					update();
					m_ups++;
				}
				m_fps++;
				m_window->update();
			}

		}

		void Scene::start()
		{
			internalInit();
			init();
			run();
		}

		void Scene::createWindow(const unsigned int &width, const unsigned int & height, const char * title, const glm::vec3& color)
		{
			Window::CreateInstance(width, height, title, color, true);
			m_window = Window::getInstance();
		}



		void Scene::internalInit()
		{
			m_renderer = new Renderer::Renderer();

			//	m_staticShader = new Shader("../Shaders/BasicShader.vs.txt", "../Shaders/BasicShader.fs.txt");
			m_particleShader = new Shader("../Shaders/particles.vs", "../Shaders/particles.fs");
			m_tex = TextureManager::LoadTexture("../Textures/body_showroom_spec.png");//TextureManager::LoadTexture("../Textures/hand_showroom_ddn.png");// // 
			m_camera = new Camera(glm::vec3(0, 0, -10.0f));
			projection = glm::perspective(glm::radians(65.f), ((float)m_window->getWidth() / (float)m_window->getHeight()), 0.1f, 1000.0f);
			tps = TPScamera(glm::vec3(0, 0, 0), projection);
			//	m_cloth = new Cloth(14, 14, 50, 50);
		}

		void Scene::internalUpdate()
		{
			m_camera->update();
			tps.update();
			//m_cloth->AddWindForce(glm::vec3(0.7f, 0.0f, 0.2f)*(float)(TIME_STEPSIZE2));
			//m_cloth->update();		
		}



		void Scene::AddEntity(Entity* entity)
		{
			m_Entities.push_back(entity);
		}

	}
}
