#include "Renderer.h"

namespace Angine {
	namespace Renderer {

		void Renderer::render(Mesh* model)
		{


			glBindVertexArray(model->getVAOID());
			if (model->getVIOID() == -1)
			{
				glDrawArrays(GL_TRIANGLES, 0, model->m_indicesCount);
				return;
			}
			glDrawElements(model->m_drawMode, model->m_indicesCount, GL_UNSIGNED_INT, NULL);
			glBindVertexArray(0);

		}



	}
}