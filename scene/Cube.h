#ifndef _CUBE
#define _CUBE

#include "Object3D.h"

/*
    @class Light
    Classe representant une lumiere
*/

class Cube : public Object3D {
    
    public:
		Cube(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
		Cube(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
		Cube(std::string strShader, vec3 c, Transform tr, Entity* p); ///Constructeur couleur
};

#endif