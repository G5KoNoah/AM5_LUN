#include "Sky.h"

// Classe representant un Sky

Sky::Sky(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Object3D(strShader, strTexture1, tr, p){
	mesh = read_mesh("../data/sky/sphere.obj");
	
}

Sky::Sky(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Object3D(strShader, strTexture1, strTexture2, tr, p){
	mesh = read_mesh("../data/sky/sphere.obj");
}

Sky::Sky(std::string strShader, vec3 c, Transform tr, Entity* p) : Object3D(strShader, c, tr, p){
	std::cout << "Sky constructeur" << std::endl;
	mesh = read_mesh("../data/sky/sphere.obj");
	std::cout << "Sky textures" << std::endl;
	skyTextures.push_back(read_texture(0, "../data/sky/tint.png"));
	std::cout << "Sky texture 0" << std::endl;
	skyTextures.push_back(read_texture(1, "../data/sky/tint2.png"));
	std::cout << "Sky texture 1" << std::endl;
	skyTextures.push_back(read_texture(2, "../data/sky/sun.png"));
	std::cout << "Sky texture 2" << std::endl;
	skyTextures.push_back(read_texture(3, "../data/sky/moon.png"));
	std::cout << "Sky texture 3" << std::endl;
	skyTextures.push_back(read_texture(4, "../data/cloud.png"));
	std::cout << "Sky texture 4" << std::endl;
	skyTextures.push_back(read_texture(5, "../data/cloud.png"));
	std::cout << "Init Sky" << std::endl;
}

void Sky::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) {

	//std::cout << "Drawing Sky" << std::endl;
	Transform mvp = Perspective(45.0f, float(1024) / 640, 0.1f, 1000.0f) * camera->view() * transform;
	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyTextures[0]); // Texture diffuse
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, skyTextures[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, skyTextures[2]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, skyTextures[3]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, skyTextures[4]);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, skyTextures[5]);
	
	program_uniform(shader, "mvpMatrix", mvp);
	program_uniform(shader, "sun_pos", vec3(0.0,1.0,0.0));
	program_uniform(shader, "tint", 0);
	program_uniform(shader, "tint2", 1);
	program_uniform(shader, "sun", 2);
	program_uniform(shader, "moon", 3);
	program_uniform(shader, "clouds1", 4);
	program_uniform(shader, "clouds2", 5);
	float weather = 1.0f; // Valeur entre 0 et 1 pour simuler le temps
	float time = SDL_cosf(SDL_GetTicks() / 1000.0f); // Temps simulé pour le cycle jour/nuit
	program_uniform(shader, "weather", weather);
	program_uniform(shader, "time", time);

	
	mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);

}

