#include "Sky.h"

// Classe representant un Sky

Sky::Sky(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Object3D(strShader, strTexture1, tr, p){
	mesh = read_mesh("../data/cube.obj");
	
}

Sky::Sky(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Object3D(strShader, strTexture1, strTexture2, tr, p){
	mesh = read_mesh("../data/cube.obj");
}

Sky::Sky(std::string strShader, vec3 c, Transform tr, Entity* p) : Object3D(strShader, c, tr, p){
	std::cout << "Sky constructeur" << std::endl;
	mesh = read_mesh("../data/cube.obj");
	std::cout << "Sky textures" << std::endl;
	skyTextures.push_back(read_texture(0, "../data/sky/sun.png"));
	std::cout << "Sky texture sun" << std::endl;
	std::cout << "Init Sky" << std::endl;
}

void Sky::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) {

	//std::cout << "Drawing Sky" << std::endl;
	Transform mvp = Perspective(45.0f, float(1024) / 640, 0.1f, 1000.0f) * camera->view() * transform;
	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyTextures[0]); // Texture diffuse

	
	program_uniform(shader, "mvpMatrix", mvp);
	program_uniform(shader, "sun_pos", dirLight->direction);

	program_uniform(shader, "sun", 2);


	
	mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);

}

