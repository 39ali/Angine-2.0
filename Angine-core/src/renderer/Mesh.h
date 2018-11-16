#pragma once
#include "../common.h"
#include "../renderer/Material.h"
#include "../renderer/VertexData.h"
#include "Texture2D.h"

namespace Angine {


		class Mesh {
		public:
			Mesh(GLuint vaoID, int indicesCount, GLuint vioID , int drawMode = GL_TRIANGLES) {
				m_vaoID = vaoID; m_indicesCount = indicesCount;
				m_vioID = vioID;
				m_drawMode = drawMode;
				m_textures = new  std::vector<Texture2D*>();
			}
			~Mesh() {}
			inline GLuint getVAOID() { return m_vaoID; };
			inline GLuint getVIOID() { return m_vioID; };
			int m_indicesCount;
			int m_drawMode;
			Material* m_material;
			///TODO : delete materials?
			std::vector<Texture2D*>* m_textures ;
			bool hasTextures = false;
		private:
			GLuint m_vaoID, m_vioID;
		};
	
}