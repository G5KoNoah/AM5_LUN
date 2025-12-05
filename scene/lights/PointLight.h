#ifndef _POINTLIGHT
#define _POINTLIGHT

#include "../Light.h"

/*
    @class PointLight
    Classe representant une lumiere ponctuelle
*/

class PointLight : public Light {

    public:
        vec3 position;

        float constant;
        float linear;
        float quadratic;
		PointLight(vec3 amb, vec3 diff, vec3 spec, float c, float l, float q, Transform tr, Entity* p);

        // mettre à jour la position à partir de la transform (appelée chaque frame si nécessaire)
        void updatePosition();
};

#endif
