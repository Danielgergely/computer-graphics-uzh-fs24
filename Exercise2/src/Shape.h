#ifndef SHAPE_H
#define SHAPE_H

#include "GLIncludes.h"

#include <vector>
#include <glm/mat4x4.hpp>


namespace cgCourse {
    class Shape {
    public:
        Shape() = default;

        virtual ~Shape();

        bool createVertexArray(GLuint posAttribLoc,
                               GLuint colAttribLoc,
                               GLuint normAttribLoc
        );

        virtual void draw() const;

        virtual std::vector<std::shared_ptr<Shape>> createMultiple(int count) const = 0;

        const glm::mat4 &getModelMatrix() const;

        const glm::vec3 &getPosition() const;

        void setPosition(const glm::vec3 &pos);

        void setRotation(float angle, const glm::vec3 &rot);

        void setScaling(const glm::vec3 &scale);

        void calculateModelMatrix();

        void setModelMatrix(glm::mat4 newModelMatrix);

    protected:
        virtual void initIndexBuffer();

        virtual void computeNormals();

        glm::mat4 rotationMatrix = glm::mat4(1);
        glm::mat4 translationMatrix = glm::mat4(1);
        glm::mat4 scalingMatrix = glm::mat4(1);
        glm::mat4 modelMatrix = glm::mat4(1);    // Identity matrix

        GLuint vaoID = 0;
        GLuint posBufferID = 0;
        GLuint colorBufferID = 0;
        GLuint normalBufferID = 0;
        GLuint indexBufferID = 0;

    public:
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> colors;
        std::vector<glm::vec3> normals;
        std::vector<glm::uvec3> faces;
        glm::vec3 objectPosition;
    };
}


#endif // SHAPE_H

