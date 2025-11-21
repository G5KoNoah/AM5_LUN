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

	m_groups = mesh.groups();
    texture = read_texture(0, "../data/DB2X2_L01.png");
	//texture_specular = read_texture(1, "../data/DB2X2_L01.png");

    texture2 = read_texture(1, "../data/bark_0021.jpg");
	//texture2_specular = read_texture(3, "../data/bark_0021.jpg");

    // corrige l'affichage du tronc : autoriser le tiling si les UV dépassent [0,1]
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);


    // etat openGL par defaut
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre

    glClearDepth(1.f);                          // profondeur par defaut
    glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
    glEnable(GL_DEPTH_TEST);                    // activer le ztest

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Arbre::Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights) {
    Transform mvp = Perspective(45.0f, float(1024) / 640, 0.1f, 1000.0f) * camera->view() * transform;
    glUseProgram(shader);
    program_uniform(shader, "mvpMatrix", mvp);

    // afficher chaque groupe
    for (unsigned i = 0; i < m_groups.size(); i++)
    {




        if (m_groups[i].index == 0) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2); // Texture diffuse
            program_uniform(shader, "texture0", 1);

        }
        else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture); // Texture diffuse
            program_uniform(shader, "texture0", 0);

        }

        // go !
        // indiquer quels attributs de Sommets du mesh sont necessaires a l'execution du shader.
        // tuto9_groups.glsl n'utilise que position et normale. les autres de servent a rien.

        // 1 draw par groupe de triangles...
        mesh.draw(m_groups[i].first, m_groups[i].n, shader, /* use position */ true, /* use texcoord */ true, /* use normal */ true, /* use color */ false, /* use material index*/ false);
    }
    
}

