#include "TPScamera.h"

namespace Angine
{
	namespace Renderer
	{
		TPScamera::TPScamera(const glm::vec3& target, const glm::mat4& projection) :Camera(projection)
		{
			m_target = target;
			m_position = glm::vec3(0, 0, 5);
		}
		TPScamera::~TPScamera()
		{


		}


		void TPScamera::update()
		{
			calculateZoom();
			calculatePitch();
			calculateAngleAroundPlayer();
			//	float horizantalDistance = m_distanceFromTarget* glm::cos(glm::radians(m_pitch));
			//	float verticalDistance = m_distanceFromTarget * glm::sin(glm::radians(m_pitch));
			//	m_position.y = m_target.x + verticalDistance;
			//	m_position.x = -horizantalDistance*glm::sin(glm::radians(m_angleAroundTarget));
			//	m_position.z = -horizantalDistance*glm::cos(glm::radians(m_angleAroundTarget));
				//m_yaw = 180 - m_angleAroundTarget;

		}

		void TPScamera::calculateZoom()
		{
			double x, y;
			Window::getMouseScroll(x, y);
			m_distanceFromTarget -= y*Core::Time::getDeltaTime() * 100;
		}
		void TPScamera::calculatePitch() // how hight the camera is from the target 
		{
			if (Window::isMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
			{
				m_pitch -= Window::getDy()*Core::Time::getDeltaTime() * 10;
				std::cout << Window::getDy() << std::endl;
			}
		}

		void TPScamera::calculateAngleAroundPlayer()
		{
			if (Window::isMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
			{
				m_angleAroundTarget -= Window::getDx() * Core::Time::getDeltaTime()*10;
			}
		}




	}
}