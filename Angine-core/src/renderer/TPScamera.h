#pragma once
#include "../common.h"
#include "../renderer/Window.h"
#include "../core/Time.h"
namespace Angine
{
	namespace Renderer
	{
		class TPScamera
		{
		public:
			TPScamera::TPScamera() {};
			TPScamera(const glm::vec3& target, const glm::mat4& projection);
			~TPScamera();
			void update();
			const glm::mat4  getMatrix()
			{
				m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(m_yaw), glm::vec3(0, 1, 0));
				m_viewMatrix = glm::translate(glm::mat4(), m_position);
				return (m_projection* m_viewMatrix);
			}
			inline const glm::vec3& getPos() { return m_position; };
		private:
			glm::vec3 m_target;
			float m_pitch = 20, m_yaw, m_roll;
			float m_distanceFromTarget = 50.0f;
			float m_angleAroundTarget = 0;
			glm::vec3 m_position;
			glm::mat4 m_viewMatrix;
			glm::mat4 m_projection;

		private:
			void calculateZoom();
			void calculatePitch();
			void calculateAngleAroundPlayer();
			float calculateHorizontalDistance();
			float calculateVerticalDistance();
			void calculateCameraPosition(float& horizDistance, float& verticalDistance);
		};


	}

}