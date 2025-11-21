#ifndef _LISTEARBRE
#define _LISTEARBRE

#include "Object3D.h"
#include "Plane.h"
#include "Arbre.h"
#include "BruitPerlin.h"
#include "Terrain.h"
#include <random>

/*
    @class Arbre
	Classe créant une liste d'arbre
*/

class Arbres {

private:
    vector<Object3D*> arbres;

public:
    //Arbres(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
    //Arbres(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
    Arbres(std::string strShader, vec3 c, Transform tr, Entity* p, Terrain* terrain); ///Constructeur couleur
	Object3D* get_tree(int i) { return arbres[i]; } ///Renvoie l'arbre d'indice i
    vector<Object3D*> make_trees(std::string strShader, vec3 c, Transform tr, Entity* p, float px, float pz, float r, int nb_arbre, float ra, Terrain * terrain); ///Créer la liste d'arbres à positions aléatoires
};

#endif