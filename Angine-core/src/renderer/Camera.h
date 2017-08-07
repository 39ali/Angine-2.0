#pragma once
#include "Window.h"
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Angine {
	namespace Renderer {
		class Camera
		{
		public:
			Camera(const glm::vec3& pos);
			~Camera();
			void update();
			const glm::mat4 & getViewMatrix()
			{
				m_viewMatrix = glm::translate(glm::mat4(), m_localPosition);
				return m_viewMatrix;
			}
			inline glm::vec3 & getPos() { return m_localPosition; };
		private:
			glm::vec3 m_localPosition = glm::vec3(0);
			glm::mat4 m_viewMatrix = glm::mat4(0);
			Window* m_win;


		};
	}
}