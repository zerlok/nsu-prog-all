#include <iostream>
#include <logger.hpp>

#include "engine.hpp"

#include "shaders/metamorphose_shader.h"
#include "shaders/screwshader.hpp"

#include "meshes/cube.hpp"
#include "meshes/strangecube.hpp"
#include "meshes/megacube.hpp"
#include "meshes/cylinder.hpp"


logger_t loggerGlobalInstance = logger_i(
		std::cout,
		logger_l::DEBUG,
		logger_d::FULL
);


int main(int argc, char *argv[])
{
	Engine engine;

	// Setup scene.
	CameraPtr camera = new Camera();
	camera->setPosition(glm::vec3(10.0, 4.0, 5.0));
	ScenePtr scene = new Scene("Lab03", camera);
	scene->setBgColor(Color::grey);

	int size = 3;
	float offset = 2.5;
	for (int z = -size; z < size+1; ++z)
	{
		for (int y = -size; y < size+1; ++y)
		{
			for (int x = -size; x < size+1; ++x)
			{
				MeshPtr c = new MegaCube();
				c->setPosition(glm::vec3(x*offset, y*offset, z*offset));
				scene->addMesh(c);
			}
		}
	}

//	scene->addMesh(new MegaCube());

	// Show scene.
	engine.setActiveScene(scene);
	engine.setDisplayFPS(true);
	engine.showScene();

	return 0;
}
