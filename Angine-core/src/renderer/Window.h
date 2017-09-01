#pragma once
#define GLEW_STATIC
#include <glew\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "../core/Time.h"
#include <glm\glm.hpp>
#define MAX_KEYS 1024
#define MAX_BUTTONS 32

namespace Angine {
	namespace Renderer {

		class Window
		{
		public:
			~Window();
			static void CreateInstance(const unsigned int width, const unsigned int height, const char* title, const glm::vec3& color, bool depth, bool fullscreen);
			inline static Window* getInstance() { return m_win; }
			inline static int getWidth() { return m_width; };
			inline static int getHeight() { return m_height; };
			inline bool isClosed()const { return m_isclosed; };
			void update();
			void clear();
			bool isResized();

			bool static isKeyPressed(unsigned int keycode);
			static bool isMouseButtonPressed(unsigned int keycode);
			void static  getMousePos(double& x, double& y) { x = mx; y = my; };
			double static getDx() { return dx; }

			double static getDy() { return dy; }

			void static  getMouseScroll(double& x, double& y) { x = xoffset; y = yoffset; yoffset = xoffset = 0; };
			void   Window::disableCursor(bool b)const;

		private:
			Window(const unsigned int width, const unsigned int height, const char* title, bool depth, bool fullScreen);
			GLFWwindow* m_window;
			int static m_width, m_height;
			const char* m_title;
			static bool m_Keys[MAX_KEYS];
			static bool m_MouseButtons[MAX_BUTTONS];
			static double mx, my, oldmx, oldmy, dy, dx;
			static double  xoffset, yoffset;//mouse wheel delta
			static bool m_isclosed;
			static bool m_isInstanciated;
			bool m_isResized;
			static Window* m_win;
			static glm::vec3 m_color;
		private:
			friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
			friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
			friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			friend void window_close_callback(GLFWwindow* window);
			friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
			friend void window_size_callback(GLFWwindow* window, int width, int height);

		};

	}
}