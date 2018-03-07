#include "Triangle.h"

float determinant(float f[3][3]) {
	return (f[0][0] * (f[1][1] * f[2][2] - f[2][1] * f[1][2]))
			- (f[1][0] * (f[2][2] * f[0][1] - f[0][2] * f[2][1]))
			+ (f[2][0] * (f[0][1] * f[1][2] - f[0][2] * f[1][1]));
}

bool Triangle::Intersect(const Ray& ray, RayHitInfo& hitInfo) const {
	float ax = this->v1->_position._data[0];
	float ay = this->v1->_position._data[1];
	float az = this->v1->_position._data[2];

	float bx = this->v2->_position._data[0];
	float by = this->v2->_position._data[1];
	float bz = this->v2->_position._data[2];

	float cx = this->v3->_position._data[0];
	float cy = this->v3->_position._data[1];
	float cz = this->v3->_position._data[2];

	float dx = ray.Direction()._data[0];
	float dy = ray.Direction()._data[1];
	float dz = ray.Direction()._data[2];

	float ox = ray.Origin()._data[0];
	float oy = ray.Origin()._data[1];
	float oz = ray.Origin()._data[2];

	float A[3][3] = { { ax - bx, ax - cx, dx }, { ay - by, ay - cy, dy }, { az
			- bz, az - cz, dz } };

	float betaInput[3][3] = { { ax - ox, ax - cx, dx },
			{ ay - oy, ay - cy, dy }, { az - oz, az - cz, dz } };
	float gammaInput[3][3] = { { ax - bx, ax - ox, dx },
			{ ay - by, ay - oy, dy }, { az - bz, az - oz, dz } };
	float tInput[3][3] = { { ax - bx, ax - cx, ax - ox }, { ay - by, ay - cy, ay
			- oy }, { az - bz, az - cz, az - oz } };

	float detA = determinant(A);
	float detBeta = determinant(betaInput);
	float detGamma = determinant(gammaInput);
	float detT = determinant(tInput);

	float beta = detBeta / detA;
	float gamma = detGamma / detA;
	float t = detT / detA;

	if (t > 0 && beta + gamma <= 1 && 0 <= beta && 0 <= gamma) {

		Vector3 o = ray.Origin();
		Vector3 d = ray.Direction();

		Vector3 a = this->v1->_position;
		Vector3 b = this->v2->_position;
		Vector3 c = this->v3->_position;

		if (t < hitInfo.Parameter) {
			hitInfo.material = this->material;
			hitInfo.Parameter = t;
			hitInfo.Position = o + (d * hitInfo.Parameter);
			hitInfo.Normal = this->normal;
			hitInfo.isHit = true;
		}

		return true;
	}

	return false;

}
