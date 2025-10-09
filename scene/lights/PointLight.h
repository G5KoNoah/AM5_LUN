#ifndef _POINTLIGHT
#define _POINTLIGHT

#include "../Light.h"

/*
    @class PointLight
    Classe representant une lumiere ?????????
*/

class PointLight : public Light {

    public:
        vec3 position;

        float constant;
        float linear;
        float quadratic;
		PointLight(vec3 amb, vec3 diff, vec3 spec, float c, float l, float q, Transform tr, Entity* p);
};

#endif
