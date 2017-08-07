#pragma once
#include <vector>
#include "Entity.h"
#include "../renderer/Window.h"
#include "Time.h"
#include <string>
#include "../renderer/Renderer.h"
#include "../renderer/Mesh.h"
#include "../renderer/Loader.h"
#include "../renderer/Shader.h"
#include "../renderer/Texture2D.h"
#include "../renderer/Camera.h"
#include "../core/Entity.h"
#include "../renderer/Model.h"
#include "../renderer/TextureManager.h"
#include "../ParticleSystem/Cloth.h"
#include "../renderer/TPScamera.h"
using namespace Angine::Renderer;
using namespace Angine::Core;
namespace Angine {
	namespace Core {
		class Scene
		{
		protected:
			Scene();
			~Scene();
			virtual void init() = 0;
			virtual void update() = 0;
			virtual void tick() = 0;
			void render();
			virtual void onRender() {};
			void AddEntity(Entity* entity);
			void run();
			void createWindow(const unsigned int &wdith, const unsigned int & height, const char* title, const glm::vec3& color = glm::vec3(0.2f, 0.3f, 0.3f));
			inline const double & getFps()const { return m_fps; };
			inline const double & getUps()const { return m_ups; };
			inline void printFpsUps() { std::cout << "fps:: " + std::to_string(getFps()) + "|| ups:: " + std::to_string(getUps()) + "\n" << std::endl; };

		private:
			std::vector<Entity*> m_Entities;
			Renderer::Window* m_window;
			float m_fps = 0.0f;
			float m_ups = 0.0f;
			Renderer::Renderer * m_renderer;
			Loader * m_loader;
			
			Shader* m_particleShader;
			Texture2D* m_tex;
			Camera * m_camera;
			//   Cloth * m_cloth;
			glm::mat4 projection ;
			TPScamera tps;
		public:
			void start();

		private:
			void internalInit();
			void internalUpdate();
		};
	}
}