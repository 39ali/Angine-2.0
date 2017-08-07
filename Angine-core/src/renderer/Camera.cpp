#include "Camera.h"

namespace Angine {
	namespace Renderer {


		Camera::Camera(const glm::vec3& pos)
		{

			m_localPosition = pos;
		};

		Camera::~Camera() {}

		void Camera::update()
		{
			if (Window::getInstance()->isKeyPressed(GLFW_KEY_D))
			{
				m_localPosition.x += 0.02f;
			}
			if (Window::getInstance()->isKeyPressed(GLFW_KEY_A))
			{
				m_localPosition.x -= 0.02f;
			}

			if (Window::getInstance()->isKeyPressed(GLFW_KEY_S))
			{
				m_localPosition.z -= 0.02f;
			}
			if (Window::getInstance()->isKeyPressed(GLFW_KEY_D))
			{
				m_localPosition.z += 0.02f;
			}

		}
	}
}