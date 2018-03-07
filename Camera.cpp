#include "Camera.h"
#include <fstream>
#include "Scene.h"
#include <limits>
#include <math.h>

double pi = 3.1415926535897;

Image Camera::Render() const {
	Image image(this->_imagePlane.Width, this->_imagePlane.Height);

	for (int i = 0; i < this->_imagePlane.Height; i++) {
		for (int j = 0; j < this->_imagePlane.Width; j++) {
			Color& color = image.Pixel(i, j);

			Vector3 m = _position + (_gaze * _imagePlane.Distance);
			Vector3 q = m
					+ ((_space.Left * _imagePlane.Left)
							+ (_space.Up * _imagePlane.Top));

			float su = (_imagePlane.Right - _imagePlane.Left) * (j + 0.5)
					/ _imagePlane.Width;
			float sv = (_imagePlane.Top - _imagePlane.Bottom) * (i + 0.5)
					/ _imagePlane.Height;

			Vector3 s = q + ((_space.Left * su) - (_space.Up * sv));

			Vector3 origin = _position;
			Vector3 direction = s - _position;
			Ray ray(origin, direction);

			color = this->rayColor(ray, CurrentScene->_rayReflect);
		}
	}

	return image;
}

Color Camera::rayColor(Ray& ray, int count) const {

	if (count < 0)
		return Color();

	Color color;

	RayHitInfo rayHitInfo;
	rayHitInfo.Parameter = std::numeric_limits<float>::max();
	rayHitInfo.isHit = false;

	std::vector<Sphere>& spheres = CurrentScene->_spheres;
	for (int k = 0; k < spheres.size(); k++) {
		Sphere& sphere = spheres[k];
		sphere.Intersect(ray, rayHitInfo);
	}

	std::vector<Mesh>& meshes = CurrentScene->_meshes;
	for (int k = 0; k < meshes.size(); k++) {
		Mesh& mesh = meshes[k];
		for (int l = 0; l < mesh.triangles.size(); l++) {
			Triangle& triangle = mesh.triangles[l];
			triangle.Intersect(ray, rayHitInfo);
		}

	}

	if (!rayHitInfo.isHit) {
		color = CurrentScene->_background;
	}

	else {
		color += rayHitInfo.material->_ambient * CurrentScene->_ambient;

		for (int k = 0; k < CurrentScene->_lights.size(); k++) {
			PointLight& pointLight = CurrentScene->_lights[k];
			Vector3 wi = (pointLight._position - rayHitInfo.Position)
					/ (pointLight._position - rayHitInfo.Position).length();

			bool pass = false;
			Ray shadowRay(rayHitInfo.Position + wi * 0.0001, wi);
			RayHitInfo shadowHitInfo;

			float tLight = (pointLight._position - shadowRay.Origin())
					/ shadowRay.Direction();

			shadowHitInfo.Parameter = std::numeric_limits<float>::max();
			shadowHitInfo.isHit = false;

			for (int k = 0; k < CurrentScene->_spheres.size(); k++) {
				Sphere& sphere = CurrentScene->_spheres[k];
				if (sphere.Intersect(shadowRay, shadowHitInfo)) {
					if (shadowHitInfo.Parameter < tLight) {
						pass = true;
						goto endFor;
					}
				}
			}

			for (int k = 0; k < CurrentScene->_meshes.size(); k++) {
				Mesh& mesh = CurrentScene->_meshes[k];
				for (int l = 0; l < mesh.triangles.size(); l++) {
					Triangle& triangle = mesh.triangles[l];
					if (triangle.Intersect(shadowRay, shadowHitInfo)) {
						if (shadowHitInfo.Parameter < tLight) {
							pass = true;
							goto endFor;
						}
					}
				}

			}

			endFor: ;

			if (!pass) {
				Vector3 wo = (_position - rayHitInfo.Position)
						/ (_position - rayHitInfo.Position).length();
				Vector3 n = rayHitInfo.Normal;
				float zero = 0;
				float cosTheta = std::max(zero, wi * n);
				Vector3 h = (wi + wo) / (wi + wo).length();
				float cosAlpha = std::max(zero, n * h);
				Color radiance = pointLight._intensity
						/ (4 * pi
								* pow(
										pointLight._position.distance(
												rayHitInfo.Position), 2));

				Vector3 wr = (wo * -1) + (n * (n * wo) * 2);
				Ray ref(rayHitInfo.Position + wr * 0.0001, wr);

				color += rayHitInfo.material->_diffuse * cosTheta * radiance;

				color += rayHitInfo.material->_specular
						* pow(cosAlpha, rayHitInfo.material->_specExp)
						* radiance;

				color += this->rayColor(ref, count - 1)
						* rayHitInfo.material->_reflectance;

			}
		}

	}

	if (color._channels[0] > 255) {
		color._channels[0] = 255;
	}

	if (color._channels[1] > 255) {
		color._channels[1] = 255;
	}

	if (color._channels[2] > 255) {
		color._channels[2] = 255;
	}

	return color;
}
