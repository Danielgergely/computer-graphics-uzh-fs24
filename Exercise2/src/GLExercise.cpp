#include "GLExercise.h"

#include "Cube.h"

#include <iostream>

namespace cgCourse
{
	GLExercise::GLExercise(glm::uvec2 _windowSize, std::string _title): GLApp(_windowSize, _title, false) {}

	bool GLExercise::init()
	{
		// Framebuffer size and window size may be different in high-DPI displays
		// setup camera with standard view (static for our case)
		cam.create(	getFramebufferSize(),
					glm::vec3(3, 3, -3),
					glm::vec3(0, 0, 0),
					glm::vec3(0, 1, 0)
					);

		programForShape = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Shape");
		programForTorusNormals = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Normals");

		// Init models
		cube = std::make_shared<Cube>();
		if(!cube->createVertexArray(0, 1, 2))
			return false;

		torus = std::make_shared<Torus>();
		if(!torus->createVertexArray(0, 1, 2))
			return false;


		// TODO: setup some initial transformation for the cube and toruses, use the implemented methods from Shape.
		// the initial state of the exercise only create one torus, you need to do modifications maybe in Shape.h to create the 6 torus.

        // create 6 toruses
        std::vector<glm::vec3> torusPositions = {
                glm::vec3(0.0, 1.4, 0.0),
                glm::vec3(0.0, -1.4, 0.0),
                glm::vec3(1.4, 0.0, 0.0),
                glm::vec3(-1.4, 0.0, 0.0),
                glm::vec3(0.0, 0.0, 1.4),
                glm::vec3(0.0, 0.0, -1.4)
        };

        std::vector<std::tuple<float, glm::vec3>> torusRotations = {
                {glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0)},
                {glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0)},
                {glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)},
                {glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)},
                {glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0)},
                {glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0)}
        };

        // rotate/scale/position all
        toruses = torus->createMultiple(6);
        for(int i = 0; i < toruses.size(); i++) {
            toruses[i]->setPosition(torusPositions[i]);
            toruses[i]->setRotation(get<0>(torusRotations[i]),get<1>(torusRotations[i]));
            toruses[i]->setScaling(glm::vec3(0.75f));

            if(!toruses[i]->createVertexArray(0, 1, 2))
                return false;

            normalsTorus = std::make_shared<MultiLine>(toruses[i]->positions, toruses[i]->normals);
            normalsTorus->setPosition(torusPositions[i]);
            normalsTorus->setRotation(get<0>(torusRotations[i]),get<1>(torusRotations[i]));
            normalsTorus->setScaling(glm::vec3(0.75f));
            if(!normalsTorus->createVertexArray(0, 1, 2))
                return false;
            normalsToruses.push_back(normalsTorus);
        }

        normalsCube = std::make_shared<MultiLine>(cube->positions, cube->normals);
        if(!normalsCube->createVertexArray(0, 1, 2))
            return false;

//
//        torus->setPosition(glm::vec3(0.0, 0.0, 1.4));
//        torus->setScaling(glm::vec3(0.75f));
//        torus->setRotation(glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));


		// Init multiline field for normals of objects
//		normalsTorus = std::make_shared<MultiLine>(torus->positions, torus->normals);
//        normalsTorus->setPosition(glm::vec3(1.0, 0.0, 0.0));
//		if(!normalsTorus->createVertexArray(0, 1, 2))
//			return false;

//        normalsTorus->setPosition(glm::vec3(0.0, 1.1, 0.0));
//        normalsTorus->setScaling(glm::vec3(0.75f));

		return true;
	}

	bool GLExercise::update()
	{
        // TODO: implement the animation of the cube and toruses
        if (scale) {
            if (scaleCounter >= 1000) {
                if (isDecreasing) {
                    scaleFactor = 1.001; // increase
                } else {
                    scaleFactor = 0.999; // decrease
                }
                isDecreasing = !isDecreasing;
                scaleCounter = 0;
            }

            for (int i = 0; i < toruses.size(); i++) {
                toruses[i]->setScaling(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
                normalsToruses[i]->setScaling(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
            }

            cube->setScaling(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
            normalsCube->setScaling(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
            scaleCounter++;
        }

        if (rotate) {

            for (int i = 0; i < toruses.size(); i++) { // fix this
                toruses[i]->setRotation(glm::radians(.2f), glm::vec3(1.0f, 1.0f, 1.0f));
                normalsToruses[i]->setRotation(glm::radians(.2f), glm::vec3(1.0f, 1.0f, 1.0f));
            }

            cube->setRotation(glm::radians(.2f), glm::vec3(1.0f, 1.0f, 1.0f));
            normalsCube->setRotation(glm::radians(.2f), glm::vec3(1.0f, 1.0f, 1.0f));
        }

		return true;
	}

	bool GLExercise::render()
	{
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		update();

		glProgramUniform1i(*programForShape, programForShape->getUniformLocation("useNormalMatrix"), useNormalMatrix);
		glProgramUniform1i(*programForShape, programForShape->getUniformLocation("normalsAsColor"), normalsAsColor);

		renderCubes();
		renderTorus();

		return true;
	}

	void GLExercise::renderCubes()
	{
		programForShape->bind();

        mvpMatrix = cam.getViewProjectionMatrix() * cube->getModelMatrix();
		// normalMatrix = TODO: compute the normal matrix
        normalMatrix = glm::transpose(glm::inverse(cube->getModelMatrix()));
		glUniformMatrix4fv(programForShape->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
		glUniformMatrix3fv(programForShape->getUniformLocation("normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
		cube->draw();

		programForShape->unbind();

        if(!drawNormals) return;

        programForTorusNormals->bind();
        mvpMatrix = cam.getViewProjectionMatrix() * normalsCube->getModelMatrix();

        glUniformMatrix4fv(programForTorusNormals->getUniformLocation("mvpMatrix"), 1,GL_FALSE, &mvpMatrix[0][0]);
        normalsCube->draw();
        programForTorusNormals->unbind();
	}

	void GLExercise::renderTorus()
	{
        for (int i = 0; i < toruses.size(); i++) {
            programForShape->bind();

            mvpMatrix = cam.getViewProjectionMatrix() * toruses[i]->getModelMatrix();
            // normalMatrix = TODO: compute the normal matrix
            normalMatrix = glm::transpose(glm::inverse(toruses[i]->getModelMatrix()));
            glUniformMatrix4fv(programForShape->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
            glUniformMatrix3fv(programForShape->getUniformLocation("normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
            toruses[i]->draw();

            programForShape->unbind();


            if(!drawNormals) continue;

            // TODO: draw the torus normals using the multiline object
            programForTorusNormals->bind();
            mvpMatrix = cam.getViewProjectionMatrix() * normalsToruses[i]->getModelMatrix();

            glUniformMatrix4fv(programForTorusNormals->getUniformLocation("mvpMatrix"), 1,GL_FALSE, &mvpMatrix[0][0]);
            normalsToruses[i]->draw();
            programForTorusNormals->unbind();
        }

	}

	bool GLExercise::end()
	{
		programForShape->deleteShaderProgramFromGPU();
		programForTorusNormals->deleteShaderProgramFromGPU();
		return true;
	}

	void GLExercise::imgui()
	{
		ImGui::SetNextWindowSize(ImVec2(200, -1));
		ImGui::Begin("status", nullptr, ImGuiWindowFlags_NoTitleBar);
		ImGui::Checkbox("drawNormals", &drawNormals);
		ImGui::Checkbox("useNormalMatrix", &useNormalMatrix);
		ImGui::Checkbox("normalsAsColor", &normalsAsColor);
        ImGui::Checkbox("scale", &scale);
        ImGui::Checkbox("rotate", &rotate);
		ImGui::End();
	}
}

