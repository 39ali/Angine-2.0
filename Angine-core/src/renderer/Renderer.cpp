#include "Renderer.h"

namespace Angine {
	namespace Renderer {

		void Renderer::render(  Mesh* model)
		{

			glBindVertexArray(model->getVAOID());
			glDrawElements(GL_TRIANGLES, model->m_indicesCount, GL_UNSIGNED_INT, NULL);
			glBindVertexArray(0);

		}



	}
}