#pragma once
#include "Camera.h"
#include <glm\/gtc\quaternion.hpp>
namespace Angine
{
	namespace Renderer
	{
		class FPSCamera : public Camera {

		public:
			FPSCamera(const float pov, const float aspectRatio, float near, float far, const  glm::vec3& pos = glm::vec3(0, 0, 10));
			~FPSCamera() {};
			void  update() override;
			const 	glm::mat4 getMatrix(void)override { return glm::lookAt(m_position, m_position + m_forward, m_up); }
			virtual const glm::mat4 & getPorjection() override
			{
				if (Window::getInstance()->isResized())
				{
					m_projection = glm::perspective(m_pov,(float) Window::getInstance()->getWidth() / (float)Window::getInstance()->getHeight(), m_near, m_far);
				}

				return m_projection;
			};
		private:
			void translate(const glm::vec3 & v) { m_position += v; };

		private:

			float m_speed, m_sprintSpeed, m_MouseSensitivity;
			float m_near, m_far, m_pov;
		};



	}



}