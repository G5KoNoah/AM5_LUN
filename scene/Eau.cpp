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
	program_uniform(shader, "mvpMatrix", mvp);
	program_uniform(shader, "time", global_time() / 1000);
	if (texture != 0) {


		program_uniform(shader, "material.diffuse", 0);
		program_uniform(shader, "material.specular", 1);
		program_uniform(shader, "material.shininess", 10.0f);

		program_uniform(shader, "model", transform);
		program_uniform(shader, "viewPos", camera->position());
		if (dirLight != nullptr) {
			program_uniform(shader, "dirLight.direction", dirLight->direction);
			program_uniform(shader, "dirLight.ambient", dirLight->ambient);
			program_uniform(shader, "dirLight.diffuse", dirLight->diffuse);
			program_uniform(shader, "dirLight.specular", dirLight->specular);
		}
		program_uniform(shader, "nbPointLights", (int)pointLights.size());
		for (int i = 0; i < pointLights.size(); i++) {
			std::string number = std::to_string(i);
			program_uniform(shader, ("pointLights[" + number + "].position").c_str(), pointLights[i]->transform);
			program_uniform(shader, ("pointLights[" + number + "].ambient").c_str(), pointLights[i]->ambient);
			program_uniform(shader, ("pointLights[" + number + "].diffuse").c_str(), pointLights[i]->diffuse);
			program_uniform(shader, ("pointLights[" + number + "].specular").c_str(), pointLights[i]->specular);
			program_uniform(shader, ("pointLights[" + number + "].constant").c_str(), pointLights[i]->constant);
			program_uniform(shader, ("pointLights[" + number + "].linear").c_str(), pointLights[i]->linear);
			program_uniform(shader, ("pointLights[" + number + "].quadratic").c_str(), pointLights[i]->quadratic);
		}
		mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);
	}
	else {
		program_uniform(shader, "color", Color(color.x, color.y, color.z, 1.0f));
		mesh.draw(shader, /* use position */ true, /* use texcoord */ (texture != 0), /* use normal */ (dirLight != nullptr || pointLights.size() > 0), /* use color */ false, /* use material index*/ true);
	}
}
