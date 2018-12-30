#include "src\renderer\Window.h"
#include "src\math\Matrix4f.h"
#include "src\renderer\CameraFP.h"
#include "src\renderer\ModelFactory.h"
#include "src\renderer\RenderContext.h"
#include "src\renderer\RenderDevice.h"
#include "src\renderer\Shader.h"
#include "src\renderer\TextureManager.h"
#include "src\renderer\TransformPipeline.h"
#include "src\renderer\VertexData.h"
#include "vendor/include/imgui/imgui.h"
#include "vendor/include/imgui/imgui_impl_glfw.h"
#include "vendor/include/imgui/imgui_impl_opengl3.h"
using namespace Angine;

struct DirectionalLight {
  vec3f color;
  float AmbientInten;
  vec3f direction;
  float diffuseInten;
};
int main() {
  const uint32 width = 1024, height = 768;
  Renderer::Window::CreateInstance(width, height, "game", vec3f(0.0, 0.0, 0.0),
                                   true, false);
  Renderer::Window *window = Renderer::Window::getInstance();
  RenderDevice device(*window);
  RenderContext context(device);
  Shader shader(device, "./res/shaders/shader.sh");

  std::vector<int> v = {1, 2, 3, 4, 5};
  std::vector<int> bb = std::move(v);

  VertexData Vertices[4] = {
      VertexData(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
      VertexData(Vector3f(0.0f, -1.0f, -1.15475f), Vector2f(0.5f, 0.0f)),
      VertexData(Vector3f(1.0f, -1.0f, 0.5773f), Vector2f(1.0f, 0.0f)),
      VertexData(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.5f, 1.0f))};

  unsigned int Indices[] = {0, 3, 1, 1, 3, 2, 2, 3, 0, 0, 1, 2};

  glUseProgram(shader.getId());

  GLuint vbo;
  GLuint vao;
  GLuint vio;
  //glfwSetInputMode(window->getWindowHandle(), GLFW_CURSOR,
  //                 GLFW_CURSOR_DISABLED);


  //// im Guiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
  ImGui::CreateContext();
  //ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window->getWindowHandle(), true);
  const char* glsl_version = "#version 150";
  ImGui_ImplOpenGL3_Init(glsl_version);

  bool isloaded;

  GLuint id = glGetUniformLocation(shader.getId(), "gsampler");
  glUniform1i(id, 0);

  Texture *tex = TextureManager::loadTexture(device, "res/textures/bricks.jpg");

  TransformPipeline transform;

  transform.setPerspectiveProj(1.f, 1000.f, width, height, 30.f);
  ;
  CameraFP camera(*window, vec3f(0, 0, -30), vec3f(0, 0, 1), vec3f(0, 1, 0));

  // transform.setRotate(vec3f(0,0,45));
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &vio);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        (const void *)offsetof(VertexData, position));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        (const void *)offsetof(VertexData, uv));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        (const void *)offsetof(VertexData, normal));

  GLuint gdlc =
      glGetUniformLocation(shader.getId(), "gDirectionalLight.ambientInten");
  glUniform1f(gdlc, 0.2f);

  GLuint gdlcc =
      glGetUniformLocation(shader.getId(), "gDirectionalLight.color");
  glUniform3fv(gdlcc, 1, (const GLfloat *)&vec3f(1));

  glUniform3fv(
      glGetUniformLocation(shader.getId(), "gDirectionalLight.direction"), 1,
      (const GLfloat *)&vec3f(1,0,0));
  glUniform1f(
      glGetUniformLocation(shader.getId(), "gDirectionalLight.diffuseInten"),
      1.0f);

  Model model;
  bool b = ModelFactory::loadModel(device, "res/models/nanosuit/nanosuit.obj",
                                   model, std::string("/textures"));

  

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glRenderMode(wireframe)
  glfwSwapInterval(1);
  float scale = 0.0f;
  vec3f worldPos;
  while (!window->isClosed()) {
    window->clear();


	//// imguii 
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("first demo");                          // Create a window called "Hello, world!" and append into it.
		if (ImGui::CollapsingHeader("Configuration")) {
			bool b;
			ImGui::Checkbox("light", &b); ImGui::SameLine(); ImGui::Text("turn light on and off");
		
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	


   
	if (window->isKeyPressed(GLFW_KEY_Q)) {
		scale += 0.5f;
		transform.setRotate(vec3f(0, scale, 0));
	}
	camera.update();
    transform.setCamera(camera.getPos(), camera.getTarget(), camera.getUp());
    //	transform.setScale(vec3f(sin(scale),sin(scale),sin(scale)));

   /* worldPos.x = sin(scale);
    worldPos.z = 5;
    transform.setWorldPos(worldPos);*/

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (const void *)offsetof(VertexData, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (const void *)offsetof(VertexData, uv));

    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "translate"), 1,
                       GL_TRUE, (const GLfloat *)transform.getTransform());
	glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "worldTransform"), 1,
		GL_TRUE, (const GLfloat *)transform.getWorldTransform());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, tex->texId);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

	context.draw(shader,model);
 



	//rendering imgui 
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//////////////////////////////////////

    window->update();
  }
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  return 0;
}
