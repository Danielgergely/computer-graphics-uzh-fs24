#ifndef GLEXERCISE_H
#define GLEXERCISE_H

#include "GLApp.h"
#include "Cube.h"
#include "Torus.h"
#include "ShaderProgram.h"
#include "MultiLine.h"
#include "Camera.h"
#include "Texture.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

namespace cgCourse
{
	struct LightInfo
	{
		glm::vec3 ambientTerm;
		glm::vec3 diffuseTerm;
		glm::vec3 specularTerm;
	};

	struct ShadowMapping
	{
		unsigned int width = 1200;
		unsigned int height = 1200;
		GLuint depthMap;
		GLuint depthMapFBO;
	};

	class GLExercise : public GLApp
	{
	public:
		enum LightMotionMode
		{
			Forward = 0, Backward = 1
		};

		GLExercise(const glm::uvec2 & windowSize, const std::string & title);

		bool init() override;
		bool update() override;
		bool render() override;
		bool end() override;

		void imgui() override;

	private:
		void addLightVariables(const std::shared_ptr<ShaderProgram> & program);
		void renderLightBox();
		void renderCubes(const glm::mat4 & lightSpaceMatrix);
		void renderTorus(const glm::mat4 & lightSpaceMatrix);
		void shadow_mapping(const glm::mat4 & lightSpaceMatrix);

		std::shared_ptr<ShaderProgram> programForShadows;
		std::shared_ptr<ShaderProgram> programForShape;
		std::shared_ptr<ShaderProgram> programForTorusNormals;
		std::shared_ptr<ShaderProgram> programForLightBox;

		Cube cube;
		Cube lightbox;
		Torus torus;
		MultiLine normalsTorus;

		std::shared_ptr<Texture> cubetex;
		std::shared_ptr<Texture> cubetexSpec;
		std::shared_ptr<Texture> cubetexNormal;
		std::shared_ptr<Texture> torustex;
		std::shared_ptr<Texture> torustexSpec;
		std::shared_ptr<Texture> torustexNormal;

		float animation = 0;
		LightMotionMode animationDir = Forward;
		glm::vec3 lightboxColor;

		glm::mat4 mvpMatrix = glm::mat4(1);

		Camera cam;
		LightInfo light;
		bool drawTorusNormals = false;
		bool displayDepthBuffer = false;

		ShadowMapping shadows;
	};
}

#endif // GLEXERCISE_H

