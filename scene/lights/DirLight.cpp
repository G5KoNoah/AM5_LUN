#include "DirLight.h"

Dirlight::Dirlight(vec3 amb, vec3 diff, vec3 spec, Transform tr, Entity* p, vec3 dir) : Light(amb, diff, spec, tr, p){
	direction = dir;

}

void Dirlight::Rotation(float angle){
	Transform R = RotationZ(angle); // ou Rotation(axis, angle);
	Vector dirVec = Vector(direction.x, direction.y, direction.z);
	// On applique la rotation à la direction
	direction = normalize(Vector(R(dirVec)));
	//if (direction.x > -1) {
	//	direction = vec3(cos(direction.x - angle), direction.y, direction.z);
	//}
	//else {
	//	direction = vec3(1, direction.y, direction.z);
	//}
}