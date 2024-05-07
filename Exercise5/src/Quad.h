//
// Created by Daniel Gergely on 5/7/24.
//

#ifndef EXERCISE_QUAD_H
#define EXERCISE_QUAD_H

#include "Shape.h"

namespace cgCourse
{
    class Quad : public Shape
    {
    public:
        Quad();
        void draw() const override;
    };
}

#endif //EXERCISE_QUAD_H
