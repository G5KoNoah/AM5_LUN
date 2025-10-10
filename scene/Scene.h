#ifndef _SCENE
#define _SCENE

#include "Object3D.h"
#include "ObjectLoad.h"
#include "Plane.h"
#include "lights/DirLight.h"
#include "lights/PointLight.h"
#include "Cube.h"

#include "mat.h"
#include "mesh.h"
#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "draw.h"

#include "app.h"        // classe Application a deriver
/*
    @class Scene
    Classe representant une scene
*/

class Scene : public App{

    private:
		Entity * base; ///Racine de la scene
        vector<Object3D*> objects; ///Liste des objets de la scene
		vector<PointLight*> pointLights; ///Liste des lumieres ponctuelles de la scene
		Dirlight* dirLight = nullptr; ///Lumiere directionnelle de la scene
        Orbiter m_camera;

    public:
		Scene() : App(1024, 640) {} ///Constructeur
        int quit(); ///Destructeur
		int init(); ///Initialise la scene
		int render(); ///Dessine la scene

};

#endif