#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H

#include <vector>
#include "Triangle.h"
#include "Material.h"

class Mesh {
public:
	Material* material;
	std::vector<Triangle> triangles;
};

#endif //RAYTRACER_MESH_H
