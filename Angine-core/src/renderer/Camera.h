#pragma once
#include "Window.h"
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../core/Time.h"
using namespace Angine::Core;
namespace Angine {
	namespace Renderer {
		class Camera
		{
		public:
			Camera(const glm::mat4& projection);
			~Camera();
			void virtual update() = 0;
			const glm::mat4  virtual getMatrix() = 0;

			inline glm::vec3 & getPos() { return m_position; };
			const glm::mat4 & getPorjection() { return m_projection; };
		protected:
			float m_pitch, m_yaw, m_roll;
			float m_distanceFromTarget = 50.0f;
			float m_angleAroundTarget = 0;
			glm::vec3 m_position;
			glm::mat4 m_viewMatrix;
			glm::mat4 m_projection;

		};
	}
}