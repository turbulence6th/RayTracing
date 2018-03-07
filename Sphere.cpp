#include "Sphere.h"
#include <math.h>

bool Sphere::Intersect(const Ray& ray, RayHitInfo& hitInfo) const {
	Vector3 d = ray.Direction();
	Vector3 o = ray.Origin();
	Vector3 c = this->center->Position();
	float R = this->radius;

	float delta = (d * (o - c) * (d * (o - c)))
			- ((d * d) * (((o - c) * (o - c)) - R * R));

	if (delta < 0)
		return false;

	float t1 = (-(d * (o - c)) + sqrt(delta)) / (d * d);
	float t2 = (-(d * (o - c)) - sqrt(delta)) / (d * d);

	float t = std::min(t1, t2);

	if (t < 0)
		return false;

	if (t < hitInfo.Parameter) {
		hitInfo.material = this->material;
		hitInfo.Parameter = t;
		hitInfo.Position = o + (d * hitInfo.Parameter);
		hitInfo.Normal = (hitInfo.Position - c) / R;
		hitInfo.isHit = true;
	}

	return true;
}
