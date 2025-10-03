#ifndef _3DOBJECT
#define _3DOBJECT

#include "Entity.h"

#include "mesh.h"

/*
    @class Object3D
    Classe representant un objet 3D
*/

class Object3D : public Entity {

    Mesh mesh; ///Mesh de l'objet

};

#endif