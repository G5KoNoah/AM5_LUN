<<<<<<< HEAD
#ifndef _Plane
#define _Plane


#include "Object3D.h"

class Plane : public Object3D {
	
	public:
		Plane(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
		Plane(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
		Plane(std::string strShader, vec3 c, Transform tr, Entity* p); ///Constructeur couleur

		Mesh makePlane(int width, int height); ///GÈnËre un plan de taille width*height
};


#endif
=======
#ifndef _PLANE
#define _PLANE

#include "Object3D.h"


/*
    @class Plane
	Classe crÈant un Plane 3D (utile pour l'eau)
*/

class Plane : public Object3D {

public:
    Plane(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
    Plane(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
    Plane(std::string strShader, vec3 c, Transform tr, Entity* p); ///Constructeur couleur
	Mesh make_plane(float width, int subdivisions); ///CrÈer un plane
};

#endif
>>>>>>> origin/sc√®ne
