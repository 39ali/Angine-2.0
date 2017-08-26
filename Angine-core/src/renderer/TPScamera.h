#pragma once
#include "../common.h"
#include "../renderer/Window.h"
#include "../core/Time.h"
#include "../renderer/Camera.h"
namespace Angine
{
	namespace Renderer
	{
		class TPScamera :public Camera
		{
		public:
			TPScamera::TPScamera() :Camera(m_projection) {};
			TPScamera(const glm::vec3& target, const glm::mat4& projection);
			~TPScamera();
			void update() override;
			const glm::mat4   getMatrix()
			{


				glm::vec4 pos = glm::rotate(glm::mat4(), glm::radians(m_pitch), glm::vec3(1, 0, 0))*
					glm::rotate(glm::mat4(), glm::radians(m_angleAroundTarget), glm::vec3(0, 1, 0))*glm::vec4(m_position, 1.0f);

				
				pos += glm::vec4(glm::normalize(glm::vec3(pos) - m_target)*m_distanceFromTarget, 0.0f);
				glm::mat4 view = glm::lookAt(glm::vec3(pos), m_target, glm::vec3(0, 1, 0));

				return (view);
			}
		
		private:
			glm::vec3 m_target;


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