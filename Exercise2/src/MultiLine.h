#ifndef MULTILINE_H
#define MULTILINE_H

#include "Shape.h"

namespace cgCourse
{
	class MultiLine : public Shape
	{
	public:
		MultiLine(	const std::vector<glm::vec3> & _vertices,
					const std::vector<glm::vec3> & _normals
					);

		void draw() const override;
        void initIndexBuffer() override;
        virtual std::vector<std::shared_ptr<Shape>> createMultiple(int count) const override{
            std::vector<std::shared_ptr<Shape>> multilines;
            for (int i = 0; i < count; ++i) {
                std::vector<glm::vec3> empty_vertices;
                std::vector<glm::vec3> empty_normals;
                std::shared_ptr<MultiLine> multiline = std::make_shared<MultiLine>(empty_vertices,
                                                                                   empty_normals);
                multilines.push_back(multiline);
            }
            return multilines;
        };

    private:
        std::vector<glm::uvec2> lineIndices;
	};
}

#endif // MULTILINE_H

