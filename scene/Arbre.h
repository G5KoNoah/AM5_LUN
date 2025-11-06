#ifndef _ARBRE
#define _ARBRE

#include "Object3D.h"

/*
    @class Arbre
    Classe representant un Arbre
*/

class Arbre : public Object3D {
    
    public:
		Arbre(std::string strShader, Transform tr, Entity* p); ///Constructeur couleur
		void Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) override; ///Dessine l'arbre

    private:
        std::vector<Color> m_colors;
};

#endif