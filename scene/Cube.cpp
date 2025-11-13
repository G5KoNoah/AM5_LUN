#include "Cube.h"

// Classe representant un cube

Cube::Cube(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Object3D(strShader, strTexture1, tr, p){
	mesh = read_mesh("../data/cube.obj");
	
}

Cube::Cube(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Object3D(strShader, strTexture1, strTexture2, tr, p){
	mesh = read_mesh("../data/source/van.obj");
}

Cube::Cube(std::string strShader, vec3 c, Transform tr, Entity* p) : Object3D(strShader, c, tr, p){
	mesh = read_mesh("../data/cube.obj");
}

