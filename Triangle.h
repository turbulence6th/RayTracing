#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "Ray.h"
#include "Vertex.h"

class Triangle
{
public:
    // This function gets a ray, and if the triangle intersects with the ray, returns true and fills the hitInfo struct
    // Else return false and leave hitInfo unchanged

	Vertex* v1;
	Vertex* v2;
	Vertex* v3;

	Material* material;

	Vector3 normal;

    bool Intersect(const Ray& ray, RayHitInfo& hitInfo) const;


};

float determinant(float f[3][3]);

#endif //RAYTRACER_TRIANGLE_H

