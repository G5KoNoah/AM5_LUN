#ifndef _TERRAIN
#define _TERRAIN

#include "Object3D.h"
#include "BruitPerlin.h"


/*
    @class Terrain
	Classe créant un terrain 3D
*/

class Terrain : public Object3D {

public:
    Terrain(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
    Terrain(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
    Terrain(std::string strShader, vec3 c, Transform tr, Entity* p); ///Constructeur couleur
	Mesh make_terrain(float width, int subdivisions, float height_max); ///Créer un terrain
	float getHeight(float x, float z); ///Renvoie la hauteur du terrain au point (x, z)
    void Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) override;
};

#endif