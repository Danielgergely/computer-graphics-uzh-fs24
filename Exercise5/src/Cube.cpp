#include "Cube.h"

#include <iostream>

namespace cgCourse
{
	glm::vec3 toPoint(int i)
	{
		return {(i & 4) >> 2, (i & 2) >> 1, (i & 1)};
	}

	Cube::Cube(float length) : Shape()
	{
		const glm::vec3 faceColors[6] = {	{ 0.8f, 0.8f, 0.0f },
											{ 0.8f, 0.0f, 0.0f },
											{ 0.0f, 0.8f, 0.0f },
											{ 0.0f, 0.0f, 0.8f },
											{ 0.0f, 0.8f, 0.8f },
											{ 0.8f, 0.0f, 0.8f }
											};

		for(int i: {0, 7})
		for(int j: {1, 2, 4})
		{
			int a = i ^ j;
			int b = i ^ ((j << 1) & 7 ? (j << 1) : 1);
			int c = (a ^ b) ^ i;

			if(i == 7) std::swap(a, b);
			for(int p: {i, a, b, c})
				positions.push_back(2 * length * (toPoint(p) - glm::vec3(0.5)));
		}

		for(auto & fc: faceColors)
		for(int i = 0; i < 4; ++i)
			colors.push_back(fc);

		for(int i = 0; i < 24; i += 4)
		{
			faces.push_back({i, i + 1, i + 2});
			faces.push_back({i + 3, i + 2, i + 1});
		}

		computeNormals();

		for(int i = 0; i < positions.size(); ++i)
			texCoords.push_back({(i & 2) >> 1, i & 1});

		for(const glm::vec3 & n: normals)
			tangents.push_back({abs(n.x) < 0.9, abs(n.x) > 0.9, 0});
	}
}

