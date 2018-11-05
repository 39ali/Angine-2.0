#include "src\renderer\Window.h"
#include "src\renderer\RenderDevice.h"
#include "src\renderer\RenderContext.h"
#include "src\renderer\Shader.h"
#include "src\math\Matrix4f.h"
#include "src\renderer\TransformPipeline.h"
#include "src\renderer\CameraFP.h"
using namespace Angine;

int main() {
	const float width = 1024, height = 768;
	std::vector<int> v = {1,2,3,4,5};
	std::vector<int > bb = std::move(v);

	Vector3f Vertices[4];
	//vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	//vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
	//vertices[2] = glm::vec3(1.0f, 1.0f, 0.0f);
	//vertices[3] = glm::vec3(-1.0f, 1.0f, 0.0f);
	Vertices[0] = Vector3f(-1.0f, -1.0f, 0.5773f);
	Vertices[1] = Vector3f(0.0f, -1.0f, -1.15475f);
	Vertices[2] = Vector3f(1.0f, -1.0f, 0.5773f);
	Vertices[3] = Vector3f(0.0f, 1.0f, 0.0f);
	//uint32 Indices[6] = {0,1,2,2,0,3};
	unsigned int Indices[] = { 0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };

	Renderer::Window::CreateInstance(width, height, "game", glm::vec3(0.0, 0.0, 0.0), true, false);
	Renderer::Window* window = Renderer::Window::getInstance();
	
	RenderDevice device(*window);
	RenderContext context(device);
	Shader shader(device ,"./res/shaders/shader.sh");
		
	GLuint vbo;
	GLuint vao;
	GLuint vio;
	glfwSetInputMode(window->getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	TransformPipeline transform;
	
	transform.setPerspectiveProj(1.f, 1000.f, width,height,30.f);
;
	CameraFP camera(*window,vec3f(0, 0, -10) , vec3f(0, 0, 1),vec3f(0,1,0));
	
	//transform.setRotate(vec3f(0,0,45));
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(Indices), Indices,GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glRenderMode(wireframe)
	glfwSwapInterval(1);
	float scale = 0.0f;
	vec3f worldPos;
	while (!window->isClosed()) {
		window->clear();
		context.draw(shader);
		scale += 0.001f;

		camera.update();
		transform.setCamera(camera.getPos(), camera.getTarget(), camera.getUp());
	//	transform.setScale(vec3f(sin(scale),sin(scale),sin(scale)));
	
		worldPos.x = sin(scale);
		worldPos.z = 5;
		transform.setWorldPos(worldPos);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//opengl needs matrix to be coloumn-major  
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "translate"),1,GL_TRUE,(const GLfloat*)transform.getTransform());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT,0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
	
	
		window->update();
	}
	
	return 0;
}