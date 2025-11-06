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



	mesh.draw(shader, /* use position */ true, /* use texcoord */ false, /* use normal */ true, /* use color */ false, /* use material index*/ true);
}

