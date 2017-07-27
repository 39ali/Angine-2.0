#include "Loader.h"


namespace Angine {
	namespace Renderer {

		RawModel* Loader::loadToVao(float* verts, int size, int * indices, int indicesSize, float * uv, int uvsize)
		{

			GLuint vaoID = createVao();
			m_vaos.push_back(vaoID);

			GLuint vio = createIndicesBuffer(indices, indicesSize);
			m_vbos.push_back(vio);
			createVertexBuffer(0, verts, size);
			createUVBuffer(1, uv, uvsize);
			unbindVao();
			return new RawModel(vaoID, indicesSize / sizeof(int), vio);
		}

		GLuint Loader::createVao()
		{
			GLuint  vao;
			glCreateVertexArrays(1, &vao);
			glBindVertexArray(vao);
			return vao;
		}
		void Loader::createVertexBuffer(int attributeNumber, float* data, int size)
		{
			GLuint buffer;
			glCreateBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			glEnableVertexAttribArray(attributeNumber);
			glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_vbos.push_back(buffer);
		}

		void Loader::createUVBuffer(int attributenumber, float* data, int size)
		{
			GLuint buffer;
			glCreateBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			glEnableVertexAttribArray(attributenumber);
			glVertexAttribPointer(attributenumber, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_vbos.push_back(buffer);
		};
		GLuint Loader::createIndicesBuffer(int * indices, int indicesSize)
		{
			GLuint indicesBufferID;
			glCreateBuffers(1, &indicesBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
			return  indicesBufferID;
		}

		void Loader::unbindVao()
		{
			glBindVertexArray(0);
		}

		Loader::~Loader()
		{
			for each (const GLuint var in m_vbos)
			{
				glDeleteBuffers(1, &var);
			}

			for each (const GLuint var in m_vaos)
			{
				glDeleteVertexArrays(1, &var);
			}

		};


	}
}