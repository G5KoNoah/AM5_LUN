#ifndef _BILLBOARD
#define _BILLBOARD


#include "Object3D.h"

class Billboard : public Object3D {
	
	public:
		Billboard(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
		Billboard(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
		Billboard(std::string strShader, vec3 c, Transform tr, Entity* p); ///Constructeur couleur

		void makePlane(int width, int height, vec3 normal, Mesh * mesh); ///Génère un plan de taille width*height
		void Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) override; ///Dessine l'objet
};


#endif
