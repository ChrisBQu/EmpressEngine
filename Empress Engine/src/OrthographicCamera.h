#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include <glm/glm.hpp>

struct OrthographicCamera {
	glm::ivec2 dimensions;
	glm::ivec2 pos;
};

#endif // ORTHOGRAPHIC_CAMERA_H