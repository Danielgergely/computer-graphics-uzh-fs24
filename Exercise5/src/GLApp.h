#ifndef GLAPP_H
#define GLAPP_H

#include "GLIncludes.h"

#include <string>

#include <glm/vec2.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


namespace cgCourse
{
	class GLApp
	{
	public:
		GLApp(const glm::uvec2 & windowSize, const std::string & title, const bool resizeable = true);
		virtual ~GLApp();

		bool run();

		virtual void keyPressHandler(int key, int action, int mods);
		virtual void mouseClickHandler(int button, int action);
		virtual void mouseMoveHandler(const glm::dvec2 & position);

	protected:
		void stop();

		virtual bool init() = 0;
		virtual bool update() = 0;
		virtual bool render() = 0;
		virtual bool end() = 0;

		virtual void imgui();

		glm::uvec2 getFramebufferSize() const;
		bool checkGlError(const char * message) const;

		GLFWwindow * window = nullptr;
		std::string title;
	};
}


#endif // GLAPP_H

