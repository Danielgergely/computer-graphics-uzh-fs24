#include "TorusKnot.h"

// This define and include will enable M_PI from math.h.
#define _USE_MATH_DEFINES
#include <math.h>

namespace cgCourse {
    TorusKnot::TorusKnot() : torusKnotSegments(50), numTubeSegments(20), p(3.0f), q(4.0f){}

    TorusKnot::TorusKnot(float offset): Shape(), torusKnotSegments(500), numTubeSegments(200),  p(10.0f), q(4.0f){
        this->calculateGeometry(offset);
    }

    void TorusKnot::calculateGeometry(float offset) {
        /* TODO: Add your code here
         * First create the torus knot mesh
         * Then offset the vertex positions along the calculated vertex normals by offset
         */
        positions.clear();
        normals.clear();
        colors.clear();
        faces.clear();

        // radius of the torus and the tube
        const float R = 8.0f;
        const float r = 3.0f;

        // generate vertices
        for (int i = 0; i <= torusKnotSegments; ++i) {
            float u = (i * M_PI * 2.0f) / torusKnotSegments;
            for (int j = 0; j <= numTubeSegments; ++j) {
                float v = (j * M_PI * 2.0f) / numTubeSegments;

                // calculate position simple torus
//                float x = (R + r * cos(v)) * cos(u);
//                float y = (R + r * cos(v)) * sin(u);
//                float z = r * sin(v);
//                glm::vec3 position = glm::vec3(x, y, z);

                // calculate positions for torus knot
                glm::vec3 position2 = calculateKnotPoint(R, r, q, p, u, v);


                // calculate normal simple torus
//                float nx = -r * cos(v) * cos(u);
//                float ny = -r * cos(v) * sin(u);
//                float nz = -r * sin(v);
//                glm::vec3 norms = glm::vec3(nx, ny, nz);

                // calculate torus knot normals
                glm::vec3 norms2 = calculateNormPoint(r, q, p, u, v);

                // offset vertex position along normal
                positions.push_back(position2 + offset * norms2);

                // set color
                colors.push_back(glm::vec3(sin(v), cos(v), 0.0f));

                // Add normal
                normals.push_back(norms2);
            }
        }

        // generate faces (triangle indices)
        for (int i = 0; i < torusKnotSegments; ++i) {
            for (int j = 0; j < numTubeSegments; ++j) {
                int nextI = (i + 1) % (torusKnotSegments + 1);
                int nextJ = (j + 1) % (numTubeSegments + 1);

                int index1 = i * (numTubeSegments + 1) + j;
                int index2 = nextI * (numTubeSegments + 1) + j;
                int index3 = i * (numTubeSegments + 1) + nextJ;
                int index4 = nextI * (numTubeSegments + 1) + nextJ;

                // first triangle
                faces.push_back(glm::uvec3(index1, index2, index3));

                // second triangle
                faces.push_back(glm::uvec3(index3, index2, index4));
            }
        }


        /* END TODO */
    }

    void TorusKnot::draw() {
        glBindVertexArray(vaoID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    /*
     * TODO: If you defined any helper methods in the header file add them here
     */

//    glm::vec3 TorusKnot::calculateNormalizedTangentVector(float r, int q, int p, float t) {
//        float scalarMultiplierTheta = 1.0f + r * cos(q * t);
//        glm::vec3 vectorPartTheta = glm::vec3(-sin(p * t), cos(p * t), 0.0);
//
//        glm::vec3 thetaResult = scalarMultiplierTheta * vectorPartTheta;
//
//        glm::vec3 vectorPartPhi = glm::vec3(-sin(q * t) * cos(p * t),
//                                            -sin(q * t) * sin(p * t),
//                                            cos(q * t));
//
//        glm::vec3 phiResult = r * vectorPartPhi;
//
//        // Cast p and q to float
//        float pf = static_cast<float>(p);
//        float qf = static_cast<float>(q);
//        glm::vec3 tangentVector = pf * thetaResult + qf * phiResult;
//        return glm::normalize(tangentVector);
//    }

    // R, r, q, p, theta, phi, range
    glm::vec3 TorusKnot::calculateKnotPoint(float R, float r, int q, int p, float u, float v) {
        // old implementation
//        glm::vec3 curve = glm::vec3((1.0 + r * cos(q * u) * cos(p * u)),
//                                    (1.0 + r * cos(q * u) * sin(p * u)),
//                                    r * sin(q * u));
//
//        glm::vec3 crossSection = glm::vec3(cos(p * u) * cos(q * u),
//                                           sin(p * u) * cos(q * u),
//                                           sin(q * u));
//
//        glm::vec3 normalizedTangentVector = calculateNormalizedTangentVector(r, q, p, u);
//
//        glm::vec3 binormal = glm::normalize(glm::cross(normalizedTangentVector, crossSection));
//
//        glm::vec3 test = curve + R * cos(v) * crossSection + R * sin(v) * binormal;

        // new implementation
        glm::vec3 position = glm::vec3(
                (R + r * cos(q * u)) * cos(p * u),
                (R + r * cos(q * u)) * sin(p * u),
                r * sin(q * u)
        );
        return position;
    }

    glm::vec3 TorusKnot::calculateNormPoint(float r, int q, int p, float u, float v) {
        // old implementation
//        glm::vec3 crossSection = glm::vec3(cos(p * u) * cos(q * u),
//                                           sin(p * u) * cos(q * u),
//                                           sin(q * u));
//        glm::vec3 normalizedTangentVector = calculateNormalizedTangentVector(r, q, p, u);
//
//        glm::vec3 binormal = glm::normalize(glm::cross(normalizedTangentVector, crossSection));
//
//        glm::vec3 test = cos(v) * crossSection + sin(v) * binormal;

        // new implementation
        glm::vec3 normal = glm::normalize(glm::vec3(
                cos(u) * cos(v),
                sin(u) * cos(v),
                sin(v)
        ));
        return normal;
    }


    /* END TODO */
}
