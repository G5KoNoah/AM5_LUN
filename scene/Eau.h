#ifndef _EAU
#define _EAU

#include "Plane.h"


/*
    @class Eau
	Classe créant un Eau 3D
*/

class Eau : public Plane {

public:
    Eau(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
    Eau(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
    Eau(std::string strShader, vec3 c, Transform tr, Entity* p); ///Constructeur couleur

	void Eau::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) override;
};

#endif