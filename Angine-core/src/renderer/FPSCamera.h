#pragma once
#include "Camera.h"
#include <glm\/gtc\quaternion.hpp>
namespace Angine
{
	namespace Renderer
	{
		class FPSCamera : public Camera {

		public:
			FPSCamera(const glm::mat4& projection);
			~FPSCamera() {};
			void  update() override;
			const 	glm::mat4 getMatrix(void)override { return glm::lookAt(m_position, m_position + m_forward, m_up); }
		private:
			void translate(const glm::vec3 & v) { m_position += v; };

		private:

			float m_speed, m_sprintSpeed, m_MouseSensitivity;
		
		};



	}



}