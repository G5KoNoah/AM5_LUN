#ifndef _ENTITY
#define _ENTITY

#include "mat.h"
#include <list>
using namespace std;

/*
    @class Entity
    Classe representant une entite
*/

class Entity{

    private:
        Transform transform; ///Position de l'entite
        Entity* parent; ///Parent de l'entite
        list<Entity*> child; ///Enfants de l'entite

};

#endif