#ifndef _DIRLIGHT
#define _DIRLIGHT

#include "../Light.h"

/*
    @class DirLight
    Classe representant une lumiere ?????????
*/

class Dirlight : public Light {

    public:
		vec3 direction;
		Dirlight(vec3 amb, vec3 diff, vec3 spec, Transform tr , Entity* p, vec3 dir );

};

#endif
