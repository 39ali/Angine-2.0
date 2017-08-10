#include "Scene.h"

namespace Angine {
	namespace Core {

		Scene::Scene()
		{

		}
		Scene::~Scene()
		{
			delete m_renderer;
			delete m_camera;

			TextureManager::Clean();
		}

		glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0.f, 0.0f, 0)) * glm::rotate(glm::mat4(), glm::radians(0.f), glm::vec3(1, 1, 1))*glm::scale(glm::mat4(), glm::vec3(0.2, 0.2, 0.2));

		void Scene::render()
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			for each (auto entity in m_Entities)
			{

				Material * mat = entity->getMaterial();
				mat->use();
				mat->setUniform("projectionView", m_camera->getPorjection() *m_camera->getMatrix());
				mat->setUniform("model", entity->transform);
				mat->setUniform("cameraPos", m_camera->getPos());
				mat->setUniform("lightPos", glm::vec3(7, 0,3));
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
				onRender();
				render();
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
			m_tex = TextureManager::LoadTexture("../Textures/body_showroom_spec.png");//TextureManager::LoadTexture("../Textures/hand_showroom_ddn.png");// // 

		}

		void Scene::internalUpdate()
		{
			m_camera->update();

		}



		void Scene::AddEntity(Entity* entity)
		{
			m_Entities.push_back(entity);
		}

	}
}
