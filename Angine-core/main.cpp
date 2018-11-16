#include "src\renderer\Window.h"
#include "src\renderer\RenderDevice.h"
#include "src\renderer\RenderContext.h"
#include "src\renderer\Shader.h"
#include "src\math\Matrix4f.h"
#include "src\renderer\TransformPipeline.h"
#include "src\renderer\CameraFP.h"
#include "src\Utils\imageLoader.h"
#include "src\renderer\VertexData.h"

using namespace Angine;


struct DirectionalLight {
	vec3f color;
	float AmbientInten;
	vec3f direction;
	float diffuseInten;
};
int main() {
	 

	const float width = 1024, height = 768;
	std::vector<int> v = {1,2,3,4,5};
	std::vector<int > bb = std::move(v);

		VertexData Vertices[4] = 
		{	VertexData (Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
			VertexData (Vector3f(0.0f, -1.0f, -1.15475f), Vector2f(0.5f, 0.0f)),
			VertexData(Vector3f(1.0f, -1.0f, 0.5773f),  Vector2f(1.0f, 0.0f)),
			VertexData(Vector3f(0.0f, 1.0f, 0.0f),      Vector2f(0.5f, 1.0f))
		};
	
		unsigned int Indices[] = { 0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };

	Renderer::Window::CreateInstance(width, height, "game", glm::vec3(0.0, 0.5, 0.5), true, false);
	Renderer::Window* window = Renderer::Window::getInstance();
	RenderDevice device(*window);
	RenderContext context(device);
	Shader shader(device, "./res/shaders/shader.sh");
	glUseProgram(shader.getId());

	GLuint vbo;
	GLuint vao;
	GLuint vio;
	glfwSetInputMode(window->getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	GLuint textureID;
	unsigned int imgw, imgh,BPP;
	FIBITMAP* map(0);
	{//textures
		BYTE* img = imageLoader("res/textures/bricks.jpg", imgw, imgh, BPP, map);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		if (BPP == 24) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgw, imgh, GL_FALSE, GL_BGR, GL_UNSIGNED_BYTE, img);
		
		}
		else if (BPP == 32)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgw, imgh, GL_FALSE, GL_BGRA, GL_UNSIGNED_BYTE, img);
		
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
		;
		GLuint id = glGetUniformLocation(shader.getId(), "gsampler");
		glUniform1i(id, 0);
	
	}


	
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offsetof(VertexData,position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, normal));

	GLuint gdlc = glGetUniformLocation(shader.getId(), "gDirectionalLight.ambientInten");
	glUniform1f(gdlc,0.3f);

	GLuint gdlcc= glGetUniformLocation(shader.getId(), "gDirectionalLight.color");	
	glUniform3fv(gdlcc,1,(const GLfloat*)& vec3f(1));
	
	glUniform3fv(glGetUniformLocation(shader.getId(), "gDirectionalLight.direction"), 1, (const GLfloat*)& vec3f(1));
	glUniform1f(glGetUniformLocation(shader.getId(), "gDirectionalLight.diffuseInten"),1);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
	
	
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offsetof(VertexData, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)offsetof(VertexData, uv));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "translate"),1,GL_TRUE,(const GLfloat*)transform.getTransform());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT,0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		window->update();
	}
	
	return 0;
}