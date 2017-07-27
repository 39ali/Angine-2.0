#pragma once
#include "Loader.h"
#include "../common.h"
#include "RawModel.h"
namespace Angine {
	namespace Renderer {

		class Loader
		{
		public:

			Loader()
			{
			};
			~Loader();
		public:
			RawModel* loadToVao(float* verts, int size, int * indices, int indicesSize, float * uv, int uvsize);

		private:
			GLuint createVao();
			void createVertexBuffer(int attributeNumber, float* data, int size);
			GLuint createIndicesBuffer(int* indices, int indicesSize);
			void createUVBuffer(int attributenumber, float* data, int size);
			std::vector<GLuint> m_vaos;
			std::vector<GLuint> m_vbos;


			void unbindVao();
		};
	}
}