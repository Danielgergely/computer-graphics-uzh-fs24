#ifndef TORUSKNOT_H
#define TORUSKNOT_H

#include "Shape.h"

namespace cgCourse
{
    class TorusKnot : public Shape
    {
    public:
        TorusKnot();
        TorusKnot(float offset);
        void calculateGeometry(float offset);
        void draw();

    private:

        // Additional properties for TorusKnot like number of segments, radii etc.
        int torusKnotSegments;
        int numTubeSegments;
        float p;
        float q;
        glm::vec3 calculateKnotPoint(float R, float r, int q, int p, float t, float s);
        glm::vec3 calculateNormPoint(float r, int q, int p, float t, float s);
//        glm::vec3 calculateNormalizedTangentVector(float r, int p, int q, float t);

    };
}

#endif //  TORUS_H
