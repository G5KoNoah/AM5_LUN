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
	program_use_texture(shader, "texture0", 0, texture);
	program_use_texture(shader, "texture1", 1, texture_specular);
}

Object3D::Object3D(std::string strShader, vec3 c, Transform tr, Entity* p) : Entity(tr, p){
	color = c;
	texture = 0;
	texture_specular = 0;
	shader = read_program(strShader.c_str());
}

Object3D::~Object3D(){
	release_program(shader);
	mesh.release();
	if(texture != 0){
		glDeleteTextures(1, &texture);
	}
	if(texture_specular != 0){
		glDeleteTextures(1, &texture_specular);
	}
	std::cout << "Destruction Object3D" << std::endl;
}

void Object3D::Draw(Transform view, Transform projection, Dirlight * dirLight, vector<PointLight*> pointLights) {
	Transform mvp = projection * view * transform;
	glUseProgram(shader);
	program_uniform(shader, "mvpMatrix", mvp);
	if(texture != 0){
		program_use_texture(shader, "texture0", 0, texture);
		if (texture_specular != 0) {
			program_use_texture(shader, "texture1", 1, texture_specular);
		}
		program_use_texture(shader, "texture0", 0, texture);
		mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);
	}
	else {
		program_uniform(shader, "color", Color(color.x, color.y, color.z, 1.0f));
		mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);
	}
	}


	

