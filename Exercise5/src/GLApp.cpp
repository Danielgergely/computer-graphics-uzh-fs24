#include "GLApp.h"

#include <glm/vec2.hpp>
#include <iostream>
#include <fstream>

#ifndef GLFW_TRUE
	#define GLFW_TRUE GL_TRUE
#endif
#ifndef GLFW_FALSE
	#define GLFW_FALSE GL_FALSE
#endif

namespace cgCourse
{
	namespace
	{
		struct EventDispatcher
		{
			static void keyCallback(GLFWwindow * window, int key, int, int action, int mods)
			{
				GLApp * app = static_cast<GLApp *>(glfwGetWindowUserPointer(window));
				if(app)
				{
					app->keyPressHandler(key, action, mods);
				}
			}

			static void mouseButtonCallback(GLFWwindow * window, int button, int action, int)
			{
				GLApp * app = static_cast<GLApp *>(glfwGetWindowUserPointer(window));
				if(app)
				{
					app->mouseClickHandler(button, action);
				}
			}

			static void cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
			{
				GLApp * app = static_cast<GLApp *>(glfwGetWindowUserPointer(window));
				if(app)
				{
					app->mouseMoveHandler({xpos, ypos});
				}
			}
		};
	} // namespace


	GLApp::GLApp(const glm::uvec2 & windowSize, const std::string & title, const bool resizeable): title(title)
	{
		// GLFW initialization
		if(!glfwInit())
		{
			std::cout << "Failed to initialize GLFW\n" << std::endl;
		}

		#ifdef __APPLE__
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#endif

		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, resizeable);

		// Window and OpenGL context creation
		window = glfwCreateWindow(int(windowSize.x), int(windowSize.y), title.c_str(), NULL, NULL);
		assert(window);

		glfwSetWindowPos(window, 64, 64);
		glfwSetWindowUserPointer(window, this);
		glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
		glfwSetKeyCallback(window, EventDispatcher::keyCallback);
		glfwSetMouseButtonCallback(window, EventDispatcher::mouseButtonCallback);
		glfwSetCursorPosCallback(window, EventDispatcher::cursorPosCallback);

		// Load OpenGL core profile functions using gl3w
		glfwMakeContextCurrent(window);

		// Initialize GLEW
		if(glewInit() != GLEW_OK)
		{
			std::cerr << "Failed to initialize GLEW\n" << std::endl;
		}

		std::cerr << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		std::cerr << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO & io = ImGui::GetIO(); (void) io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	GLApp::~GLApp()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	bool GLApp::run()
	{
		// Call custom application initialization
		if(!init())
			return false;

		// Render loop: process input handlers and call custom rendering
		while(!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			if(!render()) return false;


			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			imgui();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			glfwSwapBuffers(window);
		}

		// Call custom application cleaning and termination
		return end();
	}

	void GLApp::keyPressHandler(int key, int action, int)
	{
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			stop();
		}
	}

	void GLApp::mouseClickHandler(int, int)
	{
	}

	void GLApp::mouseMoveHandler(const glm::dvec2 &)
	{
	}

	void GLApp::stop()
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	void GLApp::imgui()
	{
		ImGui::ShowDemoWindow();
	}

	glm::uvec2 GLApp::getFramebufferSize() const
	{
		glm::ivec2 sizes(0);
		glfwGetFramebufferSize(window, &sizes.x, &sizes.y);
		return glm::uvec2(sizes);
	}

	bool GLApp::checkGlError(const char * message) const
	{
		int error = glGetError();

		if(error != GL_NO_ERROR)
		{
			std::string errorString;
			switch(error)
			{
			case GL_INVALID_ENUM:
				errorString = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				errorString = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				errorString = "GL_INVALID_OPERATION";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_OUT_OF_MEMORY:
				errorString = "GL_OUT_OF_MEMORY";
				break;
			default:
				errorString = "UNKNOWN";
				break;
			}
			std::cerr << "[" << message << "] OpenGL Error: " << errorString << std::endl;
		}
		return error == GL_NO_ERROR;
	}
}

