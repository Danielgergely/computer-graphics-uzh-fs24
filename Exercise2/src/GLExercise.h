#ifndef GLEXERCISE_H
#define GLEXERCISE_H

#include "GLApp.h"
#include "Cube.h"
#include "Torus.h"
#include "ShaderProgram.h"
#include "MultiLine.h"
#include "Camera.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

namespace cgCourse
{
	class GLExercise : public GLApp
	{
	public:
		GLExercise(glm::uvec2 _windowSize, std::string _title);

		bool init() override;
		bool update() override;
		bool render() override;
		bool end() override;
		void imgui() override;

	private:
		void renderCubes();
		void renderTorus();
        void resetShapes();

		std::shared_ptr<ShaderProgram> programForShape;
		std::shared_ptr<ShaderProgram> programForTorusNormals;

		std::shared_ptr<Cube> cube;
		std::shared_ptr<Torus> torus;
        std::vector<std::shared_ptr<Shape>> toruses;
        std::shared_ptr<MultiLine> normalsCube;
		std::shared_ptr<MultiLine> normalsTorus;
        std::vector<std::shared_ptr<MultiLine>> normalsToruses;

		glm::mat4 mvpMatrix = glm::mat4(1);
		glm::mat3 normalMatrix = glm::mat3(1);

		Camera cam;
		bool drawNormals = true;
		bool useNormalMatrix = true;
		bool normalsAsColor = false;
        bool scale = false;
        bool rotate = false;
        bool spin = false;
        bool offset = false;

        // scaling variables
        int scaleCounter = 500;
        float scaleFactor = 1.0f;
        bool isDecreasing = true;

        // rotation variables
        float rotationAngle = 0.2f;

        // torus variables
        std::vector<glm::vec3> torusPositions;
        std::vector<std::tuple<float, glm::vec3>> torusRotations;
	};
}

#endif // GLEXERCISE_H

