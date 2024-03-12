#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

namespace cgCourse
{
	class Cube : public Shape
	{
	public:
		Cube();

        virtual std::vector<std::shared_ptr<Shape>> createMultiple(int count) const override{
            std::vector<std::shared_ptr<Shape>> cubes;
            for (int i = 0; i < count; ++i) {
                std::shared_ptr<Cube> cube = std::make_shared<Cube>();
                cubes.push_back(cube);
            }
            return cubes;
        };
	};
}

#endif // CUBE_H

