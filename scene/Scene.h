#ifndef _SCENE
#define _SCENE

#include "Object3D.h"
#include "Light.h"

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
        list<Object3D*> objects; ///Liste des objets de la scene
        list<Light*> lights; ///Liste des lumieres de la scene
        Orbiter m_camera;

    public:
        int quit(); ///Destructeur
		int init(); ///Initialise la scene
		int render(); ///Dessine la scene

};

#endif