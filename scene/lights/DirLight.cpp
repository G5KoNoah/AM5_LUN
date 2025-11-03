#include "DirLight.h"

Dirlight::Dirlight(vec3 amb, vec3 diff, vec3 spec, Transform tr, Entity* p, vec3 dir) : Light(amb, diff, spec, tr, p){
	direction = dir;

}

void Dirlight::Rotation(float angle){
	if (direction.x > -1) {
		direction = vec3(direction.x - angle, direction.y, direction.z);
	}
	else {
		direction = vec3(1, direction.y, direction.z);
	}
}