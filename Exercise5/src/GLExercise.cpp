#include "GLExercise.h"

#include <iostream>


namespace cgCourse {
    GLExercise::GLExercise(const glm::uvec2 &windowSize, const std::string &title) : GLApp(windowSize, title, false) {
        normalsTorus = MultiLine(torus.positions, torus.normals, torus.tangents, torus.texCoords);
    }

    bool GLExercise::init() {
        cam.create(getFramebufferSize(),
                   glm::vec3(3, 3, -3),
                   glm::vec3(0, 1, 0),
                   glm::vec3(0, 1, 0)
        );

        programForShadows = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Shadows");
        programForShape = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Shape");
        programForTorusNormals = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Normals");
        programForLightBox = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/Lightbox");

        if (!lightbox.createVertexArray(0, 1, 2, 3, 4))
            return false;

        lightbox.setPosition(glm::vec3(0.0, 0.5, -2.0));
        lightbox.setScaling(glm::vec3(0.05, 0.05, 0.05));
        lightboxColor = glm::vec3(1.0, 1.0, 1.0);

        if (!cube.createVertexArray(0, 1, 2, 3, 4))
            return false;

        cube.setPosition(glm::vec3(-2, -1.5, 5));
        cube.setScaling(glm::vec3(2.5, 2.5, 2.5));

        if (!torus.createVertexArray(0, 1, 2, 3, 4))
            return false;

        torus.setPosition(glm::vec3(-1, 0, 0));

        if (!normalsTorus.createVertexArray(0, 1, 2, 3, 4))
            return false;


        light.ambientTerm = {1, 1, 1};
        light.diffuseTerm = {1, 1, 1};
        light.specularTerm = {1, 1, 1};


        cubetex = std::make_shared<Texture>();
        cubetex->loadFromFile(std::string(RES_DIR) + "/container.png");

        cubetexSpec = std::make_shared<Texture>();
        cubetexSpec->loadFromFile(std::string(RES_DIR) + "/container_specular.png");

        cubetexNormal = std::make_shared<Texture>();
        cubetexNormal->loadFromFile(std::string(RES_DIR) + "/container_normal.jpg");

        torustex = std::make_shared<Texture>();
        torustex->loadFromFile(std::string(RES_DIR) + "/brickwall.jpg");

        torustexNormal = std::make_shared<Texture>();
        torustexNormal->loadFromFile(std::string(RES_DIR) + "/brickwall_normal.jpg");

        torustexSpec = std::make_shared<Texture>();
        torustexSpec->loadFromFile(std::string(RES_DIR) + "/brickwall_specular.jpg");


        // TODO: ShadowMapping depth buffer and texture initialization, use shadows struct.

        // 2D texture -> framebuffer's depth buffer
        glGenTextures(1, &shadows.depthMap);
        glBindTexture(GL_TEXTURE_2D, shadows.depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadows.width, shadows.height,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, shadows.depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadows.depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return true;
    }

    bool GLExercise::update() {
        static float ta = 0;
        static float tc = 0;

        ta += 1;
        tc += 0.5;

        torus.setRotation(ta, glm::vec3(1.0f, 1.0f, 1.0f));
        cube.setRotation(tc, glm::vec3(0, 1.0f, 0));

        if (animationDir == Forward) {
            if (animation > 1.5)
                animationDir = Backward;
            else
                animation += 0.01;
        } else {
            if (animation < -2.0)
                animationDir = Forward;
            else
                animation -= 0.01;
        }

        lightbox.setPosition(glm::vec3(animation, 1, -2));

        return true;
    }

    bool GLExercise::render() {
        update();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);


        // TODO: compute lightSpaceMatrix
        glm::vec3 lightPos = lightbox.objectPosition;
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        float near_plane = 1.0f, far_plane = 7.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        shadow_mapping(lightSpaceMatrix);

//        if (displayDepthBuffer)
            return true;


        renderLightBox();

        addLightVariables(programForShape);

        renderCubes(lightSpaceMatrix);
        renderTorus(lightSpaceMatrix);

        return true;
    }

    void GLExercise::shadow_mapping(const glm::mat4 &lightSpaceMatrix) {
        if (!displayDepthBuffer) {
            glViewport(0, 0, shadows.width, shadows.height);
            glBindFramebuffer(GL_FRAMEBUFFER, shadows.depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
        }

        programForShadows->bind();

        mvpMatrix = lightSpaceMatrix * cube.modelMatrix;
        glUniformMatrix4fv(programForShadows->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
        cube.draw();

        mvpMatrix = lightSpaceMatrix * torus.modelMatrix;
        glUniformMatrix4fv(programForShadows->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
        torus.draw();

        programForShadows->unbind();

        if (!displayDepthBuffer) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    void GLExercise::addLightVariables(const std::shared_ptr<ShaderProgram> &program) {
        program->bind();
        glUniform3fv(program->getUniformLocation("camPos"), 1, &cam.getPosition()[0]);
        glUniform3fv(program->getUniformLocation("light.position"), 1, &lightbox.objectPosition[0]);
        glUniform3fv(program->getUniformLocation("light.ambient"), 1, &light.ambientTerm[0]);
        glUniform3fv(program->getUniformLocation("light.diffuse"), 1, &light.diffuseTerm[0]);
        glUniform3fv(program->getUniformLocation("light.specular"), 1, &light.specularTerm[0]);
        program->unbind();
    }

    void GLExercise::renderCubes(const glm::mat4 &lightSpaceMatrix) {
        programForShape->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubetex->getTexHandle());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cubetexSpec->getTexHandle());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, cubetexNormal->getTexHandle());

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadows.depthMap);

        glUniform1i(programForShape->getUniformLocation("shapetex"), 0);
        glUniform1i(programForShape->getUniformLocation("shapetexSpec"), 1);
        glUniform1i(programForShape->getUniformLocation("shapetexNormal"), 2);
        glUniform1i(programForShape->getUniformLocation("shadows"), 3);

        mvpMatrix = cam.getViewProjectionMatrix() * cube.modelMatrix;
        glUniformMatrix4fv(programForShape->getUniformLocation("modelMatrix"), 1, GL_FALSE, &cube.modelMatrix[0][0]);
        glUniformMatrix4fv(programForShape->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
        glUniformMatrix4fv(programForShape->getUniformLocation("lightSpaceMatrix"), 1, GL_FALSE,
                           &lightSpaceMatrix[0][0]);
        cube.draw();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, 0);

        programForShape->unbind();
    }

    void GLExercise::renderTorus(const glm::mat4 &lightSpaceMatrix) {
        programForShape->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, torustex->getTexHandle());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, torustexSpec->getTexHandle());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, torustexNormal->getTexHandle());

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadows.depthMap);

        glUniform1i(programForShape->getUniformLocation("shapetex"), 0);
        glUniform1i(programForShape->getUniformLocation("shapetexSpec"), 1);
        glUniform1i(programForShape->getUniformLocation("shapetexNormal"), 2);
        glUniform1i(programForShape->getUniformLocation("shadows"), 3);

        mvpMatrix = cam.getViewProjectionMatrix() * torus.modelMatrix;
        glUniformMatrix4fv(programForShape->getUniformLocation("modelMatrix"), 1, GL_FALSE, &torus.modelMatrix[0][0]);
        glUniformMatrix4fv(programForShape->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
        glUniformMatrix4fv(programForShape->getUniformLocation("lightSpaceMatrix"), 1, GL_FALSE,
                           &lightSpaceMatrix[0][0]);
        torus.draw();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, 0);

        programForShape->unbind();


        if (!drawTorusNormals) return;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, torustex->getTexHandle());

        glUniform1i(programForShape->getUniformLocation("linestex_normal"), 0);

        programForTorusNormals->bind();
        mvpMatrix = cam.getViewProjectionMatrix() * torus.modelMatrix;
        glUniformMatrix4fv(programForTorusNormals->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
        normalsTorus.draw();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        programForTorusNormals->unbind();
    }

    void GLExercise::renderLightBox() {
        programForLightBox->bind();
        mvpMatrix = cam.getViewProjectionMatrix() * lightbox.modelMatrix;
        glUniform3fv(programForLightBox->getUniformLocation("objectColor"), 1, &lightboxColor[0]);
        glUniformMatrix4fv(programForLightBox->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
        lightbox.draw();
        programForLightBox->unbind();
    }

    bool GLExercise::end() {
        return true;
    }

    void GLExercise::imgui() {
        ImGui::SetNextWindowSize(ImVec2(200, -1));
        ImGui::Begin("status", nullptr, ImGuiWindowFlags_NoTitleBar);
        ImGui::Checkbox("drawTorusNormals", &drawTorusNormals);
        ImGui::Checkbox("displayDepthBuffer", &displayDepthBuffer);
        ImGui::End();
    }
}

