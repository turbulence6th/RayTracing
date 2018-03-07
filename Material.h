#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "Color.h"
#include "Vector3.h"

class Scene;

class Material {
    //////////////////
    /// PUT YOUR VARIABLES, HELPER FUNCTIONS HERE !
    //////////////////
public:

    /*
     * Implement a Calculate function that does Diffuse, Specular and Ambient, Reflective shading
     */
	int materialId;
	Color _ambient;
	Color _diffuse;
	Color _specular;
	float _specExp;
	Color _reflectance;

    friend std::istream& operator>>(std::istream& stream, Material& mat);
};

using MaterialId = int;

#endif //RAYTRACER_MATERIAL_H
