#include "src\renderer\Window.h"
#include "src\renderer\RenderDevice.h"
#include "src\renderer\RenderContext.h"
#include "src\renderer\Shader.h"
#include "src\math\Matrix4f.h"
using namespace Angine;

int main() {
	glm::vec3 vertices[4];
	vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
	vertices[2] = glm::vec3(1.0f, 1.0f, 0.0f);
	vertices[3] = glm::vec3(-1.0f, 1.0f, 0.0f);

	uint32 Indices[6] = {0,1,2,2,0,3};

	Renderer::Window::CreateInstance(1024, 768, "game", glm::vec3(0.0, 0.0, 0.0), true, false);
	Renderer::Window* window = Renderer::Window::getInstance();
	
	RenderDevice device(*window);
	RenderContext context(device);
	Shader shader(device ,"./res/shaders/shader.sh");
		
	GLuint vbo;
	GLuint vao;
	GLuint vio;

	mat4f m;

	


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(Indices), Indices,GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	float scale = 0.0f;
	while (!window->isClosed()) {
		window->clear();
		context.draw(shader);
		scale += 0.001f;

		//m.makeTranslate(vec3f(sin(scale), 0, 0));
		m.makeScale(vec3f(sin(scale),sin(scale),sin(scale)));
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//opengl needs matrix to be coloumn-major  
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "translate"),1,GL_TRUE, m);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
	
	
		window->update();
	}
	
	return 0;
}