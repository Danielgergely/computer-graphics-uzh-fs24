#ifndef TORUS_H
#define TORUS_H

#include "Shape.h"

namespace cgCourse {
    class Torus : public Shape {
    public:
        Torus();

        void addTwoFaces(unsigned int _idx1, unsigned int _idx2, unsigned int _idx3, unsigned int _idx4);

        virtual std::vector<std::shared_ptr<Shape>> createMultiple(int count) const override{
            std::vector<std::shared_ptr<Shape>> toruses;
            for (int i = 0; i < count; ++i) {
                std::shared_ptr<Torus> torus = std::make_shared<Torus>();
                toruses.push_back(torus);
            }
            return toruses;
        };
    };


}

#endif // TORUS_H

