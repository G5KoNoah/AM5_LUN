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

    protected:
        Entity* parent; ///Parent de l'entite
        list<Entity*> child; ///Enfants de l'entite
    public:
        Transform transform; ///Position de l'entite
		Entity(); ///Constructeur par defaut
        Entity(Transform t, Entity * p); ///Constructeur
        ~Entity(); ///Destructeur
		void addChild(Entity* e); ///Ajoute un enfant a l'entite
		void ChangeTransform(Transform t); ///Change la transformation de l'entite et l'applique a ses enfants
};

#endif