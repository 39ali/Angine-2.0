#include "FPSCamera.h"

namespace Angine
{
	namespace Renderer
	{

		FPSCamera::FPSCamera(const glm::mat4& projection) :Camera(projection), m_speed(10), m_sprintSpeed(m_speed * 3), m_MouseSensitivity(2)
		{
			Window::getInstance()->disableCursor(true);
			m_forward = glm::vec3(0, 0, -1);
			m_right = glm::vec3(1, 0, 0);
			m_up = glm::vec3(0, 1, 0);
		}

		void  FPSCamera::update()
		{

			m_yaw -= Window::getDx() * m_MouseSensitivity * Time::getDeltaTime();
			m_pitch += Window::getDy() * m_MouseSensitivity * Time::getDeltaTime();

			m_forward.y = glm::sin(glm::radians(m_pitch));
			m_forward.x = glm::cos(glm::radians(m_pitch))*glm::cos(glm::radians(m_yaw));
			m_forward.z = glm::cos(glm::radians(m_pitch)) * glm::sin(glm::radians(m_yaw));
			m_forward = glm::normalize(m_forward);


			m_right = glm::normalize(glm::cross(m_forward, m_up));

			float dt = (float)Time::getDeltaTime();
			float sp = (Window::isKeyPressed(GLFW_KEY_LEFT_SHIFT)) ? m_sprintSpeed : m_speed;
			sp *= dt;
			if (Window::isKeyPressed(GLFW_KEY_W))
			{
				translate(m_forward*sp);
			}
			if (Window::isKeyPressed(GLFW_KEY_S))
			{
				translate(-m_forward*sp);
			}

			if (Window::isKeyPressed(GLFW_KEY_A))
			{
				translate(-m_right*sp);
			}
			if (Window::isKeyPressed(GLFW_KEY_D))
			{
				translate(m_right*sp);
			}

		}


	}
}