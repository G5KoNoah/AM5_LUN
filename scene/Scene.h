#ifndef _SCENE
#define _SCENE

#include "Object3D.h"
#include "Light.h"

/*
    @class Scene
    Classe representant une scene
*/

class Scene{

    private:
        list<Object3D> objects; ///Liste des objets de la scene
        list<Light> lights; ///Liste des lumieres de la scene

};

#endif