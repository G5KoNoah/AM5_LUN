#ifndef _OCEAN
#define _OCEAN

#include "Plane.h"


/*
    @class Eau
	Classe cr�ant un Eau 3D
*/

class Ocean : public Plane {

public:
    Ocean(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
    Ocean(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
    Ocean(std::string strShader, vec3 c, Transform tr, Entity* p); ///Constructeur couleur

	void Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) override;
};

#endif