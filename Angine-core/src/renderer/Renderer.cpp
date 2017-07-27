#include "Renderer.h"

namespace Angine {
	namespace Renderer {

		void Renderer::render(const RawModel* model)const
		{
			glBindVertexArray(model->m_vaoID);
			glDrawElements(GL_TRIANGLES, model->m_indicesCount, GL_UNSIGNED_INT, NULL);
			glBindVertexArray(0);

		}



	}
}