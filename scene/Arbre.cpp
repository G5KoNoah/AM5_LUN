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

    // dessiner m_objet avec le shader program
    // configurer le pipeline 
    glUseProgram(shader);

    // configurer le shader program
    // . recuperer les transformations
    Transform model = transform;
    Transform view = camera->view();
    Transform projection = Perspective(45.0f, float(1024) / 640, 0.1f, 1000.0f);

    // . composer les transformations : model, view et projection
    Transform mv = view * model;
    Transform mvp = projection * mv;

    // . parametrer le shader program :
    //   . transformation : la matrice declaree dans le vertex shader s'appelle mvpMatrix
    program_uniform(shader, "mvpMatrix", mvp);
    program_uniform(shader, "mvMatrix", mv);

    //   . ou, directement en utilisant openGL :
    //   int location= glGetUniformLocation(program, "mvpMatrix");
    //   glUniformMatrix4fv(location, 1, GL_TRUE, mvp.buffer());

    // . parametres "supplementaires" :
    //   . couleur diffuse des matieres, cf la declaration 'uniform vec4 materials[];' dans le fragment shader
    int location = glGetUniformLocation(shader, "materials");
    glUniform4fv(location, m_colors.size(), &m_colors[0].r);

    // go !
    // indiquer quels attributs de sommets du mesh sont necessaires a l'execution du shader.
    // tuto9_materials.glsl n'utilise que position et material_index. les autres de servent a rien.
    // 1 draw pour tous les triangles de l'objet.
    mesh.draw(shader, /* use position */ true, /* use texcoord */ false, /* use normal */ true, /* use color */ false, /* use material index*/ true);

    
}

