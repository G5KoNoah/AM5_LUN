#ifndef _OBJECT_LOAD
#define _OBJECT_LOAD


#include "Object3D.h"

class ObjectLoad : public Object3D {
	
	public:
		ObjectLoad(std::string strShader, std::string strTexture1, Transform tr, Entity* p, std::string path); ///Constructeur une seule texture
		ObjectLoad(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p, std::string path); ///Constructeur deux textures
		ObjectLoad(std::string strShader, vec3 c, Transform tr, Entity* p, std::string path); ///Constructeur couleur
};


#endif
