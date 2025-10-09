#include "PointLight.h"

PointLight::PointLight(vec3 amb, vec3 diff, vec3 spec, float c, float l, float q, Transform tr, Entity* p) : Light(amb, diff, spec, tr, p){
	position = vec3(transform.m[0][3], transform.m[1][3], transform.m[2][3]);
	constant = c;
	linear = l;
	quadratic = q;
}