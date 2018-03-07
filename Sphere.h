#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "Ray.h"
#include "Vertex.h"

class Sphere {

public:
    // This function gets a ray, and if the sphere intersects with the ray, returns true and fills the hitInfo struct
    // Else return false and leave hitInfo unchanged
	Material* material;
	float radius;
	Vertex* center;
    bool Intersect(const Ray& ray, RayHitInfo& hitInfo) const;
};


#endif //RAYTRACER_SPHERE_H
