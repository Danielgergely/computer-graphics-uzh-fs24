#include "GLExample.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace cgCourse
{
	GLExample::GLExample(glm::uvec2 _windowSize, std::string _title): GLApp(_windowSize, _title, false) {}

	bool GLExample::init()
	{
		// Framebuffer size and window size may be different in high-DPI displays
		// setup camera with standard view (static for our case)
		cam.create(	getFramebufferSize(),
					glm::vec3(5, 5, -5),
					glm::vec3(0, 0, 0),
					glm::vec3(0, 1, 0)
					);

		programForLightBox = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Lightbox");
        programForShape = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Shape");
        programForNormal = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Normals");

		// create box for light position
		lightbox = std::make_shared<Cube>();
		if(!lightbox->createVertexArray(0, 1, 2))
			return false;

		lightbox->setPosition(glm::vec3(0.0, 0.5, -1.0));
		lightbox->setScaling(glm::vec3(0.05, 0.05, 0.05));

		// Init models

        // create textured cube
		cube = std::make_shared<Cube>();
		if(!cube->createVertexArray(0, 1, 2))
			return false;

        cube->setPosition(glm::vec3(-1.5, 0.5, 3.5));
        cube->setScaling(glm::vec3(1.5, 1.5, 1.5));

        // create torus
		torus = std::make_shared<Torus>();
		if(!torus->createVertexArray(0, 1, 2))
			return false;

		torus->setPosition(glm::vec3(3.0, 0.0, 1));
		torus->setScaling(glm::vec3(1.5, 1.5, 1.5));

        // create torus knot
        torusKnot = std::make_shared<TorusKnot>();
        if(!torusKnot->createVertexArray(0, 1, 2))
            return false;
        torusKnot->setRotation(90.0, glm::vec3(1.0, 0.0, 0.0));
        torusKnot->setPosition(glm::vec3(-1.0, -1, -3.0));
        torusKnot->setScaling(glm::vec3(2.0, 2.0, 2.0));

		// Init multiline field for normals of objects
        normalsTorus = std::make_shared<MultiLine>(torus->positions,
                                                   torus->normals);
        if(!normalsTorus->createVertexArray(0, 1, 2))
            return false;

		normalsTorusKnot = std::make_shared<MultiLine>(torusKnot->positions,
                                                   torusKnot->normals);
		if(!normalsTorusKnot->createVertexArray(0, 1, 2))
			return false;

		return true;
	}

	bool GLExample::update()
	{
        if (stopAnimation)
            return true;

		torusKnot->setRotation(glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		if(animationDir == Forward)
		{
			if(animation > 4.0) {
                animationDir = Backward;
            } else {
                animation += 0.01;
            }
		}
		else
		{
			if(animation < -5.0) {
                animationDir = Forward;
            } else {
                animation -= 0.01;
            }
		}

		lightbox->setPosition(glm::vec3(animation, 1.5, -0.0));

		return true;
	}

	bool GLExample::render()
	{
		glEnable(GL_DEPTH_TEST);

		// Clear the color and depth buffers
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		update();

		renderLightBox();

        addLightVariables(programForShape);

        if (drawCube) renderCube();
        if (drawTorus) renderTorus();
        if (drawTorusKnot) renderTorusKnot();

		return true;
	}

    /* TODO update this function to set the light information for the
	 * shaders. Don't forget that shader have to be bound before shader uniform can be set..
	 */
	void GLExample::addLightVariables(const std::shared_ptr<ShaderProgram> & program)
	{
        // shader program binding
		program->bind();

        // slides version
        // view Matrix
//        glm::mat4 viewMatrix = cam.getViewMatrix();
//        glUniformMatrix4fv(program->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
//
//        // setting the light position
//        glm::vec3 lightPos = lightbox->getPosition();
//        glUniform4f(program->getUniformLocation("light.position"), lightPos[0], lightPos[1], lightPos[2], 1.0f);
//
//        // light color setting
//        glUniform4f(program->getUniformLocation("light.color"), lightColor[0], lightColor[1], lightColor[2], 1.0f);
//
//        glUniform1f(program->getUniformLocation("material.ambient"), 0.20f);
//        glUniform1f(program->getUniformLocation("material.diffuse"), 0.20f);
//        glUniform1f(program->getUniformLocation("material.specular"), 0.8);
//
//        glUniform1f(program->getUniformLocation("material.shininess"), 16.0f);

        // V2
        glm::vec3 lightPos = lightbox->getPosition();
        glUniform3f(program->getUniformLocation("light.position"), lightPos[0], lightPos[1], lightPos[2] );

        // light color setting
        glUniform3f(program->getUniformLocation("light.color"), lightColor[0], lightColor[1], lightColor[2]);


        // light properties
        glUniform3f(program->getUniformLocation("material.ambient"), 0.30f, 0.30f, 0.30f);
        glUniform3f(program->getUniformLocation("material.diffuse"), 0.60f, 0.60f, 0.60f);
        glUniform3f(program->getUniformLocation("material.specular"), 0.8, 0.8, 0.8);
        glUniform1f(program->getUniformLocation("material.shininess"), 32.0f);

        // viewer position
        glUniform3f(program->getUniformLocation("viewPosition"), cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);


        // ********************** bonus task ******************************
        if(bonusTask) {
            // Define spotlight parameters
            glm::vec3 spotlightPosition = glm::vec3(0.0f, 6.0f, -10.f);
            glm::vec3 spotlightDirection = glm::vec3(0.0f, 0.5f, -1.0f);
            float spotlightInnerAngle = glm::cos(glm::radians(12.0f));
            float spotlightOuterAngle = glm::cos(glm::radians(10.0f));

            // Pass spotlight parameters to shader
            glUniform3fv(programForShape->getUniformLocation("spotlight.position"), 1, &spotlightPosition[0]);
            glUniform3fv(programForShape->getUniformLocation("spotlight.direction"), 1, &spotlightDirection[0]);
            glUniform1f(programForShape->getUniformLocation("spotlight.innerAngle"), spotlightInnerAngle);
            glUniform1f(programForShape->getUniformLocation("spotlight.outerAngle"), spotlightOuterAngle);
        }
        // unbinding the shader program
        program->unbind();

	}
    // END TODO

    void GLExample::renderCube() {
        programForShape->bind();
        mvpMatrix = cam.getViewProjectionMatrix() * cube->getModelMatrix();
        glUniformMatrix4fv(programForShape->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
        glUniformMatrix4fv(programForShape->getUniformLocation("modelMatrix"), 1, GL_FALSE, &cube->getModelMatrix()[0][0]);
        glUniform1i(programForShape->getUniformLocation("gouraudShading"), renderMode == GOURAUD_SHADING);
        glUniform1i(programForShape->getUniformLocation("bonusTask"), bonusTask);
        cube->draw();
        programForShape->unbind();
    }

    void GLExample::renderTorus() {
        programForShape->bind();
        mvpMatrix = cam.getViewProjectionMatrix() * torus->getModelMatrix();
        glUniformMatrix4fv(programForShape->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
        glUniformMatrix4fv(programForShape->getUniformLocation("modelMatrix"), 1, GL_FALSE, &torus->getModelMatrix()[0][0]);
        glUniform1i(programForShape->getUniformLocation("gouraudShading"), renderMode == GOURAUD_SHADING);
        glUniform1i(programForShape->getUniformLocation("bonusTask"), bonusTask);
        torus->draw();
        programForShape->unbind();

        if(drawTorusNormals) {
            programForNormal->bind();
            mvpMatrix = cam.getViewProjectionMatrix() * torus->getModelMatrix();
            glUniformMatrix4fv(programForNormal->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
            normalsTorus->draw();
            programForNormal->unbind();
        }
    }

    void GLExample::renderTorusKnot() {
        programForShape->bind();
        mvpMatrix = cam.getViewProjectionMatrix() * torusKnot->getModelMatrix();
        glUniformMatrix4fv(programForShape->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
        glUniformMatrix4fv(programForShape->getUniformLocation("modelMatrix"), 1, GL_FALSE, &torusKnot->getModelMatrix()[0][0]);
        glUniform1i(programForShape->getUniformLocation("gouraudShading"), renderMode == GOURAUD_SHADING);
        glUniform1i(programForShape->getUniformLocation("bonusTask"), bonusTask);
        torusKnot->draw();
        programForShape->unbind();

        if(drawTorusKnotNormals) {
            programForNormal->bind();
            mvpMatrix = cam.getViewProjectionMatrix() * torusKnot->getModelMatrix();
            glUniformMatrix4fv(programForNormal->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
            normalsTorusKnot->draw();
            programForNormal->unbind();
        }
    }

	void GLExample::renderLightBox()
	{
		programForLightBox->bind();
		mvpMatrix = cam.getViewProjectionMatrix() * lightbox->getModelMatrix();
		glUniform3fv(programForLightBox->getUniformLocation("objectColor"), 1, &lightColor[0]);
		glUniformMatrix4fv(programForLightBox->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
		lightbox->draw();
		programForLightBox->unbind();
	}

	bool GLExample::end()
	{
		programForShape->deleteShaderProgramFromGPU();
		programForNormal->deleteShaderProgramFromGPU();
        programForLightBox->deleteShaderProgramFromGPU();
        return true;
	}

    void GLExample::imgui()
    {
        bool open = true;
        ImGui::Begin("Controls", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);

        // Light Color
        ImGui::ColorEdit3("Light Color", lightColor);
        ImGui::Checkbox("Pause Animation", &stopAnimation);
        ImGui::Separator();
        ImGui::RadioButton("Gouraud Shading", &renderMode, GOURAUD_SHADING);
        ImGui::RadioButton("Phong Shading", &renderMode, PHONG_SHADING);
        ImGui::Separator();
        ImGui::Checkbox("Render Cube", &drawCube);
        ImGui::Checkbox("Render Torus", &drawTorus);
        ImGui::Checkbox("Render Torus Knot", &drawTorusKnot);
        ImGui::Separator();
        ImGui::Checkbox("Render Torus Normals", &drawTorusNormals);
        ImGui::Checkbox("Render Torus Knot Normals", &drawTorusKnotNormals);
        ImGui::Separator();
        ImGui::Checkbox("Bonus Task", &bonusTask);
        ImGui::End();
    }
}

