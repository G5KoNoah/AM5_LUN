#include "Object3D.h"

Object3D::Object3D(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Entity(tr, p){
	texture = read_texture(0, strTexture1.c_str());
	texture_specular = 0;
	shader = read_program(strShader.c_str());
}

Object3D::Object3D(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Entity(tr, p){
	texture = read_texture(0, strTexture1.c_str());
	texture_specular = read_texture(1, strTexture2.c_str());
	shader = read_program(strShader.c_str());
}

Object3D::Object3D(std::string strShader, vec3 c, Transform tr, Entity* p) : Entity(tr, p){
	color = c;
	texture = 0;
	texture_specular = 0;
	shader = read_program(strShader.c_str());
}

Object3D::~Object3D(){
	release_program(shader);
	if(texture != 0){
		glDeleteTextures(1, &texture);
	}
	if(texture_specular != 0){
		glDeleteTextures(1, &texture_specular);
	}
}

void Object3D::Draw(Transform view, Transform projection) {

}

