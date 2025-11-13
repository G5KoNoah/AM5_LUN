#include "Arbre.h",

// Classe representant un arbre

Arbre::Arbre(std::string strShader, Transform tr, Entity* p) : Object3D(strShader, vec3(0.0f, 0.0f, 0.0f), tr, p) {
	mesh = read_mesh("../data/Tree.obj");

	if (mesh.materials().count() == 0) {
		// pas de matieres, pas d'affichage
		std::cout << "Erreur chargement dans les matériaux" << std::endl;
	}

	m_colors.resize(16);


	// charger les textures

	const Materials& materials = mesh.materials();
	assert(materials.count() <= int(m_colors.size()));
	for (int i = 0; i < materials.count(); i++)
		m_colors[i] = materials.material(i).diffuse;

}

void Arbre::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) {
	Transform mvp = Perspective(45.0f, float(1920) / 1080, 0.1f, 1000.0f) * camera->view() * transform;
	glUseProgram(shader);
		
	program_uniform(shader, "mvpMatrix", mvp);

	int location = glGetUniformLocation(shader, "materials");
	glUniform4fv(location, m_colors.size(), &m_colors[0].r);

	program_uniform(shader, "material.diffuse", 0);
	if (texture_specular != 0) {
		program_uniform(shader, "material.specular", 1);
	}
	else {
		program_uniform(shader, "material.specular", 0);
	}
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



	mesh.draw(shader, /* use position */ true, /* use texcoord */ true, /* use normal */ true, /* use color */ false, /* use material index*/ true);
}

