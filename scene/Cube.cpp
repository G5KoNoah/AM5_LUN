#include "Cube.h"

// Classe representant un cube

Cube::Cube(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Object3D(strShader, strTexture1, tr, p){
	mesh = read_mesh("../data/cube.obj");
	
}

