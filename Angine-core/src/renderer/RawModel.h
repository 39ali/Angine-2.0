#pragma once
#include "../common.h"
namespace Angine {
	namespace Renderer {


		class RawModel {
		public:
			RawModel(GLuint vaoID, int indicesCount, GLuint vioID) {
				m_vaoID = vaoID; m_indicesCount = indicesCount;
				m_vioID = vioID;
			}
			~RawModel() {}

			GLuint m_vaoID, m_vioID;
			int m_indicesCount;
		};
	}
}