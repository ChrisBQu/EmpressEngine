#ifndef TRANSFORM_STRUCT_H
#define TRANSFORM_STRUCT_H

#include <glm/glm.hpp>

struct TransformStruct {
	glm::ivec2 pos;
	glm::ivec2 scale;
	int depth;
	float rotation;
	float alpha;
};

#endif