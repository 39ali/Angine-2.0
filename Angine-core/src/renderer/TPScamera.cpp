#include "TPScamera.h"

namespace Angine
{
	namespace Renderer
	{
		TPScamera::TPScamera(const glm::vec3& target, const glm::mat4& projection)
		{
			m_target = target;
			m_projection = projection;
		}
		TPScamera::~TPScamera()
		{


		}

		float TPScamera::calculateHorizontalDistance()
		{
			return m_distanceFromTarget * glm::cos(glm::radians(m_pitch));
		}
		float TPScamera::calculateVerticalDistance()
		{
			return m_distanceFromTarget * glm::sin(glm::radians(m_pitch));
		}

		void TPScamera::calculateCameraPosition(float& horizDistance, float& verticalDistance)
		{

			float offsetx = horizDistance*glm::sin(glm::radians(m_angleAroundTarget));
			float offsetz = horizDistance*glm::cos(glm::radians(m_angleAroundTarget));
			m_position.x = m_target.x - offsetx;
			m_position.z = m_target.z - offsetz;
			m_position.y = m_target.y + verticalDistance;
		}


		void TPScamera::update()
		{
			calculateZoom();
			calculatePitch();
			calculateAngleAroundPlayer();
			float horDistance = calculateHorizontalDistance();
			float verDistance = calculateVerticalDistance();
			calculateCameraPosition(horDistance, verDistance);
			m_yaw = 180 - (m_angleAroundTarget);
		}

		void TPScamera::calculateZoom()
		{
			double x, y;
			Window::getMouseScroll(x, y);
			m_distanceFromTarget -= y*Core::Time::getDeltaTime();
		}
		void TPScamera::calculatePitch() // how hight the camera is from the target 
		{
			if (Window::isMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
			{
				m_pitch -= Window::getDy()*Core::Time::getDeltaTime();
				std::cout << Window::getDy() << std::endl;
			}
		}

		void TPScamera::calculateAngleAroundPlayer()
		{
			if (Window::isMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
			{
				m_angleAroundTarget -= Window::getDx() * Core::Time::getDeltaTime();
			}
		}




	}
}