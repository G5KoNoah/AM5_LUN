#ifndef _LIGHT
#define _LIGHT

#include "Entity.h"

/*
    @class Light
    Classe representant une lumiere
*/

class Light : public Entity {

    public:
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
		Light(vec3 amb, vec3 diff, vec3 spec, Transform tr = Identity(), Entity* p = nullptr);

};

#endif