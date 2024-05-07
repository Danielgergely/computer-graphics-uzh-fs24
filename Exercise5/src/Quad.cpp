//
// Created by Daniel Gergely on 5/7/24.
//
#include "Quad.h"

namespace cgCourse {
    Quad::Quad() : Shape() {
        // positions and texture coordinates
        this->positions = {
                glm::vec3(-1.0f, 1.0f, 0.0f),
                glm::vec3(-1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(-1.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, -1.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 0.0f)
        };

        this->texCoords = {
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f)
        };

        // faces
        this->faces = {
                glm::uvec3(0, 1, 2),
                glm::uvec3(3, 4, 5)
        };

        // compute normals
        this->computeNormals();
    }

    void Quad::draw() const
    {
        glBindVertexArray(this->vaoID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}
