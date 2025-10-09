#include "Light.h"

Light::Light(vec3 amb, vec3 diff, vec3 spec, Transform tr, Entity* p) : Entity(tr, p){
	ambient = amb;
	diffuse = diff;
	specular = spec;
}