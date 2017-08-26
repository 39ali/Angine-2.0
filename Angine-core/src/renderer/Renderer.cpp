#include "Renderer.h"

namespace Angine {
	namespace Renderer {

		void Renderer::render(Mesh* model, Core::RenderMode mode)
		{
			renderMode(mode);

			glBindVertexArray(model->getVAOID());
			if (model->getVIOID() == -1)
			{
				glDrawArrays(GL_TRIANGLES, 0, model->m_indicesCount);
				return;
			}
			glDrawElements(model->m_drawMode, model->m_indicesCount, GL_UNSIGNED_INT, NULL);
			glBindVertexArray(0);

		}

		void Renderer::renderMode(Core::RenderMode mode)const
		{
			if (mode == Core::RenderMode::Line)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else if (mode == Core::RenderMode::Fill)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}



	}
}