#include "Eau.h"

// Classe representant de l'eau

Eau::Eau(std::string strShader, std::string strTexture1, Transform tr, Entity* p) : Plane(strShader, strTexture1, tr, p) {
}

Eau::Eau(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p) : Plane(strShader, strTexture1, strTexture2, tr, p) {
}

Eau::Eau(std::string strShader, vec3 c, Transform tr, Entity* p) : Plane(strShader, c, tr, p) {
}

void Eau::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) {
	Transform mvp = Perspective(45.0f, float(1920) / 1080, 0.1f, 1000.0f) * camera->view() * transform;
	glUseProgram(shader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflection); // Reflection
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refraction); // Refraction

	program_uniform(shader, "reflection", 0);
	program_uniform(shader, "refraction", 1);

	program_uniform(shader, "mvpMatrix", mvp);

	mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);

	if (reflection != 0 && refraction != 0) {

		

		

	}
	else {
		cout << "Probleme dans Eau car else : Voir Eau.cpp" << endl;
		//program_uniform(shader, "color", Color(color.x, color.y, color.z, 1.0f));
		//mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);
	}
}
