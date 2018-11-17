#include "Window.h"

namespace Angine {
	namespace Renderer {

		bool  Window::m_Keys[MAX_KEYS];
		bool  Window::m_MouseButtons[MAX_BUTTONS];
		double Window::mx, Window::my, Window::oldmx, Window::oldmy;
		bool  Window::m_isclosed;
		bool Window::m_isInstanciated = false;
		Window* Window::m_win = nullptr;
		vec3f Window::m_color;
		int  Window::m_width, Window::m_height;
		double Window::xoffset, Window::yoffset;
		double  Window::dx, Window::dy;
		Window::Window(const unsigned int width, const unsigned int height, const char* title, bool depth, bool fullScreen) :
			m_title(title)
		{
			m_width = width;  m_height = height;
			for each(auto k in m_Keys)
			{
				k = false;
			}

			for each (auto b in m_MouseButtons)
			{
				b = false;
			}


			glfwInit();
			
			//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

			if (fullScreen)
			{
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				m_width = mode->width;
				m_height = mode->height;
				m_window = glfwCreateWindow(m_width, m_height, m_title, monitor, nullptr);
				std::cout << m_width << ",,," << m_height << "/n";
			}
			else
				m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
			if (m_window == nullptr)
			{
				std::cout << "didn't create window" << std::endl;
				glfwTerminate();
			}
		//	glfwMakeContextCurrent(m_window);

		//	glewExperimental = GL_TRUE;
			/*if (glewInit() != 0)
			{
				std::cout << "glew init error" << std::endl;
			}

			if (depth) {
				glEnable(GL_DEPTH_TEST);
			}*/

			//int m_width, m_height;

			glfwGetFramebufferSize(m_window, &m_width, &m_height);
			glViewport(0, 0, m_width, m_height);

			glfwSetWindowUserPointer(m_window, this);

			glfwSetCursorPosCallback(m_window, cursor_position_callback);
			glfwSetMouseButtonCallback(m_window, mouse_button_callback);
			glfwSetKeyCallback(m_window, key_callback);
			glfwSetWindowCloseCallback(m_window, window_close_callback);
			glfwSetScrollCallback(m_window, scroll_callback);
			glfwSetWindowSizeCallback(m_window, window_size_callback);
			glfwSwapInterval(1);

		}

		void Window::CreateInstance(const unsigned int width, const unsigned int height, const char* title, const vec3f& color, bool depth, bool fullscreen)
		{
			m_color = color;
			if (m_isInstanciated) { return; }

			m_isInstanciated = true;
			m_win = new Window(width, height, title, depth, fullscreen);
		}

		void Window::update()
		{
			dx = oldmx - mx;
			dy = oldmy - my;
			oldmx = mx;
			oldmy = my;

			glfwSwapBuffers(m_window);
			Core::Time::updateTime(glfwGetTime());
			Core::Time::updateFps(glfwGetTime());
		}
		void Window::clear()
		{
			//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClearColor(m_color.x, m_color.y, m_color.z, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glfwPollEvents();
		}



		bool Window::isKeyPressed(unsigned int keycode)
		{
			if (keycode >= MAX_KEYS)
				return false;

			return m_Keys[keycode];
		}

		bool Window::isMouseButtonPressed(unsigned int button)
		{
			if (button >= MAX_BUTTONS)
				return false;

			return m_MouseButtons[button];
		}

		void Window::disableCursor(bool b)const
		{
			if (b == true) {
				glfwSetCursorPos(m_window, m_width, m_height);
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}


		Window::~Window()
		{
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}





		void key_callback(GLFWwindow* win, int key, int scancode, int action, int mode)
		{
			Window* window = (Window*)glfwGetWindowUserPointer(win);

			window->m_Keys[key] = (action != GLFW_RELEASE);
		}
		static int i = 0;
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			if (i == 0)
			{
				i++;
				win->oldmx = xpos;
				win->oldmy = xpos;
				win->mx = xpos;
				win->my = ypos;
			}
			else {
				win->mx = xpos;
				win->my = ypos;
			}
		}

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);

			win->m_MouseButtons[button] = (action != GLFW_RELEASE);
		}


		void window_close_callback(GLFWwindow* window)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_isclosed = true;
		}

		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->xoffset = xoffset;
			win->yoffset = yoffset;
		}
		void window_size_callback(GLFWwindow* window, int width, int height)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_width = width;
			win->m_height = height;
			glViewport(0, 0, width, height);
			win->m_isResized = true;
		}

		bool Window::isResized()
		{
			if (m_isResized)
			{
				m_isResized = false;
				return true;
			}
			return false;
		}


	}
}