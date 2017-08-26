#include "Camera.h"

namespace Angine {
	namespace Renderer {


		Camera::Camera(const glm::mat4& projection) :m_projection(projection), m_position(glm::vec3(0))
		{
			m_yaw = -90.f;
			m_pitch = 0.0f;
			 m_roll = 0;

		};

		Camera::~Camera() {}


	}
}