#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include <vector>
#include "Vector3.h"
#include "Image.h"
#include "Sphere.h"

class Camera {
public:
	struct {
		Vector3 Forward, Up, Left;
	} _space;

	struct {
		float Left, Right, Bottom, Top;
		float Distance;

		int Width;
		int Height;

	} _imagePlane;

	Vector3 _position;
	Vector3 _gaze;

	std::string _outputFile;

public:

	/*
	 * BEWARE! You should render each camera by using this function, we will grade it according to regulations!
	 */
	Image Render() const;

	Color rayColor(Ray& ray, int count) const;

	const std::string& OutputFile() const {
		return _outputFile;
	}
};

#endif //RAYTRACER_CAMERA_H
