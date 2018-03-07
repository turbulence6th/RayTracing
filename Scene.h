#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include "Camera.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Color.h"
#include "PointLight.h"
#include "Vertex.h"

class Scene {
public:
	std::vector<Vertex> _vertices;

	std::vector<Camera> _cameras;
	std::vector<Sphere> _spheres;
	std::vector<Mesh> _meshes;

	std::vector<Material> _materials;

	std::vector<PointLight> _lights;

	int _rayReflect;
	Color _background;
	Color _ambient;

public:

	const std::vector<Camera>& Cameras() const {
		return _cameras;
	}

	//////////////////////////////
	// READ SCENE FUNCTIONS HERE !
	//////////////////////////////
};

extern Scene* CurrentScene;

inline void ReadScene(int argc, char** argv) {
	CurrentScene = new Scene();

	std::fstream camera_input(argv[1]);
	int count_camera;
	camera_input >> count_camera;

	for (int i = 0; i < count_camera; i++) {
		std::string camera_type, camera_id;
		camera_input >> camera_type;
		camera_input >> camera_id;

		Camera* camera = new Camera();
		camera_input >> camera->_position._data[0];
		camera_input >> camera->_position._data[1];
		camera_input >> camera->_position._data[2];

		camera_input >> camera->_gaze._data[0];
		camera_input >> camera->_gaze._data[1];
		camera_input >> camera->_gaze._data[2];

		camera->_space.Forward._data[0] = -camera->_gaze._data[0];
		camera->_space.Forward._data[1] = -camera->_gaze._data[1];
		camera->_space.Forward._data[2] = -camera->_gaze._data[2];

		camera_input >> camera->_space.Up._data[0];
		camera_input >> camera->_space.Up._data[1];
		camera_input >> camera->_space.Up._data[2];

		camera->_space.Left = camera->_space.Up.crossProduct(
				camera->_space.Forward);

		camera_input >> camera->_imagePlane.Left;
		camera_input >> camera->_imagePlane.Right;
		camera_input >> camera->_imagePlane.Bottom;
		camera_input >> camera->_imagePlane.Top;
		camera_input >> camera->_imagePlane.Distance;
		camera_input >> camera->_imagePlane.Width;
		camera_input >> camera->_imagePlane.Height;

		camera_input >> camera->_outputFile;

		CurrentScene->_cameras.push_back(*camera);
	}

	std::fstream scene_input(argv[2]);
	scene_input >> CurrentScene->_rayReflect;

	scene_input >> CurrentScene->_background._channels[0];
	scene_input >> CurrentScene->_background._channels[1];
	scene_input >> CurrentScene->_background._channels[2];

	scene_input >> CurrentScene->_ambient._channels[0];
	scene_input >> CurrentScene->_ambient._channels[1];
	scene_input >> CurrentScene->_ambient._channels[2];

	int point_light_count;
	scene_input >> point_light_count;
	for (int i = 0; i < point_light_count; i++) {
		PointLight* pointLight = new PointLight;
		scene_input >> pointLight->_position._data[0];
		scene_input >> pointLight->_position._data[1];
		scene_input >> pointLight->_position._data[2];

		scene_input >> pointLight->_intensity._channels[0];
		scene_input >> pointLight->_intensity._channels[1];
		scene_input >> pointLight->_intensity._channels[2];

		CurrentScene->_lights.push_back(*pointLight);
	}

	int material_count;
	scene_input >> material_count;
	for (int i = 0; i < material_count; i++) {
		std::string material_type;
		scene_input >> material_type;

		Material* material = new Material();
		scene_input >> material->materialId;

		scene_input >> material->_ambient._channels[0];
		scene_input >> material->_ambient._channels[1];
		scene_input >> material->_ambient._channels[2];

		scene_input >> material->_diffuse._channels[0];
		scene_input >> material->_diffuse._channels[1];
		scene_input >> material->_diffuse._channels[2];

		scene_input >> material->_specular._channels[0];
		scene_input >> material->_specular._channels[1];
		scene_input >> material->_specular._channels[2];
		scene_input >> material->_specExp;

		scene_input >> material->_reflectance._channels[0];
		scene_input >> material->_reflectance._channels[1];
		scene_input >> material->_reflectance._channels[2];

		CurrentScene->_materials.push_back(*material);
	}

	int vertex_count;
	scene_input >> vertex_count;
	std::string vertex_type, vertex_data;
	scene_input >> vertex_type;
	scene_input >> vertex_data;
	for (int i = 0; i < vertex_count; i++) {
		Vertex* vertex = new Vertex();
		vertex->vertexId = i + 1;

		scene_input >> vertex->_position._data[0];
		scene_input >> vertex->_position._data[1];
		scene_input >> vertex->_position._data[2];

		CurrentScene->_vertices.push_back(*vertex);
	}

	int model_count;
	scene_input >> model_count;
	for (int i = 0; i < model_count; i++) {
		std::string model_type;
		scene_input >> model_type;

		if (model_type == "#Mesh") {
			std::string mesh_id;
			scene_input >> mesh_id;

			Mesh* mesh = new Mesh();

			int triangle_count;
			scene_input >> triangle_count;

			int materialId;
			scene_input >> materialId;
			for (int j = 0; j < CurrentScene->_materials.size(); j++) {
				if (CurrentScene->_materials[j].materialId == materialId) {
					mesh->material = &CurrentScene->_materials[j];
				}
			}

			for (int j = 0; j < triangle_count; j++) {
				Triangle* triangle = new Triangle();
				triangle->material = mesh->material;
				int vertexId;
				scene_input >> vertexId;
				triangle->v1 = &CurrentScene->_vertices[vertexId - 1];
				scene_input >> vertexId;
				triangle->v2 = &CurrentScene->_vertices[vertexId - 1];
				scene_input >> vertexId;
				triangle->v3 = &CurrentScene->_vertices[vertexId - 1];
				triangle->normal =
						(triangle->v2->Position() - triangle->v1->Position()).crossProduct(
								triangle->v3->Position()
										- triangle->v1->Position())
								/ (triangle->v2->Position()
										- triangle->v1->Position()).crossProduct(
										triangle->v3->Position()
												- triangle->v1->Position()).length();
				mesh->triangles.push_back(*triangle);
			}

			CurrentScene->_meshes.push_back(*mesh);
		}

		if (model_type == "#Sphere") {
			std::string sphere_id;
			scene_input >> sphere_id;

			Sphere* sphere = new Sphere();
			int materialId;
			scene_input >> materialId;
			for (int j = 0; j < CurrentScene->_materials.size(); j++) {
				if (CurrentScene->_materials[j].materialId == materialId) {
					sphere->material = &CurrentScene->_materials[j];
				}
			}

			scene_input >> sphere->radius;

			int vertexId;
			scene_input >> vertexId;
			sphere->center = &CurrentScene->_vertices[vertexId - 1];

			CurrentScene->_spheres.push_back(*sphere);
		}
	}
}

#endif //RAYTRACER_SCENE_H
