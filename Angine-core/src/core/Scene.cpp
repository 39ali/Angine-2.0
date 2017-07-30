#include "Scene.h"

namespace Angine {
	namespace Core {

		Scene::Scene()
		{

		}
		Scene::~Scene()
		{
			delete m_renderer;
			delete m_shader;
			TextureManager::Clean();
		}

		void Scene::render()
		{
			m_shader->use();
			glActiveTexture(GL_TEXTURE0);
			m_tex->bind();

			glm::mat4 projection = glm::perspective(glm::radians(65.f), ((float)m_window->getWidth() / (float)m_window->getHeight()), 0.1f, 1000.0f);
			//glm::mat4 projection = glm::ortho(0.0f, (float)m_window->getWidth(), 0.0f, (float)m_window->getHeight(), 0.1f, 1000.f);
			m_shader->setUniform("projection", projection);
			m_shader->setUniform("view", m_camera->getViewMatrix());
			for each (auto entity in m_Entities)
			{
				m_shader->setUniform("model", entity->transform);
				for each (Mesh*  mesh in entity->getModel()->m_meshes)
				{
					m_renderer->render(mesh);
				}

			}
		

			m_shader->unuse();

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

		void Scene::createWindow(const unsigned int &width, const unsigned int & height, const char * title)
		{
			Window::CreateInstance(width, height, title);
			m_window = Window::getInstance();
		}



		void Scene::internalInit()
		{
			m_renderer = new Renderer::Renderer();
			
			m_shader = new Shader("../Shaders/BasicShader.vs.txt", "../Shaders/BasicShader.fs.txt");
			
			m_tex = TextureManager::LoadTexture("../Textures/body_showroom_spec.png");//TextureManager::LoadTexture("../Textures/hand_showroom_ddn.png");// // 
			m_camera = new Camera(glm::vec3(0, 0, -10.0f));
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
