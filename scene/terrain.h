#ifndef _TERRAIN
#define _TERRAIN

#include "BruitPerlin.h"
#include "Object3D.h"

/*
    @class Terrain
	Classe créant un terrain 3D 
*/

class Terrain : public Object3D {

public:
    //Terrain(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
    //Cube(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
    Terrain(std::string strShader, vec3 c, Transform tr, Entity* p); ///Constructeur couleur
};

#endif