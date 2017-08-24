#pragma once
#include "Material.h"
#include "Shader.h"
#include "Camera.h"
namespace Angine {
	namespace Renderer {
		class BasicLightingMaterial :public Material
		{
		public:
			BasicLightingMaterial(Shader * shader, Camera * cam) :Material(shader), m_cam(cam)
			{


			}
			~BasicLightingMaterial();

			void onRender() override
			{
				setUniform("cameraPos", m_cam->getPos());
			}
		private:
			Camera * m_cam;
		};
	}
}