#include "Eau.h"

// Classe representant de l'eau

Eau::Eau(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Plane(strShader, strTexture1, tr, p) {
	mesh = makePlane(100, 100);
	std::cout << "Creation de mauvaise Eau 2" << std::endl;
}

Eau::Eau(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Plane(strShader, strTexture1, strTexture2, tr, p) {
	mesh = makePlane(100, 100);
	std::cout << "Creation de mauvaise Eau 1" << std::endl;
}

Eau::Eau(std::string strShader, vec3 c, Transform tr, Entity* p) : Plane(strShader, c, tr, p) {
	//mesh = makePlane(100, 100);
	//transform = tr;
	//parent = p;
	//shader = read_program(strShader.c_str());
	//std::cout << "Creation de Eau" << std::endl;
}

void Eau::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) {
	//std::cout << "Commence Passage dans le Draw de Eau" << std::endl;
	Transform mvp = Perspective(45.0f, float(1920) / 1080, 0.1f, 1000.0f) * camera->view() * transform;
	glUseProgram(shader);
	//std::cout << "Passage dans le Draw de Eau" << std::endl;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflection); // Reflection
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refraction); // Refraction

	//cout << "Reflection eau dans eau " << reflection << endl;
	//cout << "Refraction eau dans eau " << refraction << endl;
	cout << shader << endl;

	program_uniform(shader, "reflextionTexture", 0);
	program_uniform(shader, "refraxtionTexture", 1);

	program_uniform(shader, "mvpMatrix", mvp);

	mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ false);

	if (reflection != 0 && refraction != 0) {
		//cout << "Bien dans Eau" << endl;
	}
	else {
		cout << "Probleme dans Eau car else : Voir Eau.cpp" << endl;
	}
}

void Eau::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights, GLuint waterShader) {
	//std::cout << "Commence Passage dans le Draw de Eau" << std::endl;
	Transform mvp = Perspective(45.0f, float(1920) / 1080, 0.1f, 1000.0f) * camera->view() * transform;
	glUseProgram(waterShader);
	//std::cout << "Passage dans le Draw de Eau" << std::endl;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflection); // Reflection
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refraction); // Refraction

	cout << "Reflection eau dans eau " << reflection << endl;
	cout << "Refraction eau dans eau " << refraction << endl;
	//cout << waterShader << endl;

	program_uniform(waterShader, "reflectionTexture", 0);
	program_uniform(waterShader, "refractionTexture", 1);

	program_uniform(waterShader, "mvpMatrix", mvp);

	mesh.draw(waterShader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ false);

	if (reflection != 0 && refraction != 0) {
		//cout << "Bien dans Eau" << endl;
	}
	else {
		cout << "Probleme dans Eau car else : Voir Eau.cpp" << endl;
	}
}

void Eau::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights, GLuint reflec, GLuint refrac) {
	std::cout << "Commence Passage dans le Draw 2 de Eau" << std::endl;
	Transform mvp = Perspective(45.0f, float(1920) / 1080, 0.1f, 1000.0f) * camera->view() * transform;
	glUseProgram(shader);
	std::cout << "Passage dans le Draw 2 de Eau" << std::endl;

	reflection = reflec;
	refraction = refrac;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflection); // Reflection
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refraction); // Refraction

	program_uniform(shader, "reflectionTexture", 0);
	program_uniform(shader, "refraxtionTexture", 1);

	program_uniform(shader, "mvpMatrix", mvp);

	mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ false);

	if (reflection != 0 && refraction != 0) {
		//cout << "Bien dans Eau" << endl;
	}
	else {
		cout << "Probleme dans Eau car else : Voir Eau.cpp" << endl;
	}
}
