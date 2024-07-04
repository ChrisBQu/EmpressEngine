#ifndef SOLID_OBJECT_H
#define SOLID_OBJECT_H

#include "Geometry.h"

class SolidObject {
public:
	SolidObject();
	GeometryRectangle aabb;
};

#endif // SOLID_OBJECT_H