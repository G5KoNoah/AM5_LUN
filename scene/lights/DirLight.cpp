#include "DirLight.h"

Dirlight::Dirlight(vec3 amb, vec3 diff, vec3 spec, Transform tr, Entity* p, vec3 dir) : Light(amb, diff, spec, tr, p){
	direction = dir;
}