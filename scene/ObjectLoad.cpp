#include "ObjectLoad.h"

ObjectLoad::ObjectLoad(std::string strShader, std::string strTexture1, Transform tr, Entity* p, std::string path) : Object3D(strShader, strTexture1, tr, p){
	mesh = read_mesh(path.c_str());
}

ObjectLoad::ObjectLoad(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p, std::string path) : Object3D(strShader, strTexture1, strTexture2, tr, p){
	mesh = read_mesh(path.c_str());
}

ObjectLoad::ObjectLoad(std::string strShader, vec3 c, Transform tr, Entity* p, std::string path) : Object3D(strShader, c, tr, p){
	mesh = read_mesh(path.c_str());
}