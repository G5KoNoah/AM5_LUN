#include "Scene.h"


int Scene::quit(){
    for(int i=0; i<objects.size(); i++){
		delete objects[i];
	}
	objects.clear();
    std::cout << "Destruction Scene" << std::endl;
    for(int i=0; i<pointLights.size(); i++){
		delete pointLights[i];
		pointLights.pop_back();
	}
	pointLights.clear();
	std::cout << "Destruction Lights" << std::endl;
	if (dirLight != nullptr)
        delete dirLight;
	std::cout << "Destruction DirLight" << std::endl;
    delete base;
	cout << "Destruction App" << endl;
	return 0;   // ras, pas d'erreur
}
int Scene::init(){

	base = new Entity();
	// Creation d'une lumiere
	dirLight = new Dirlight(vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), Identity()* Translation(vec3(5.0,5.0,5.0)), base, vec3(-0.2f, -1.0f, -0.3f));
	//pointLights.push_back(new PointLight(vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), 1.0f, 0.09f, 0.032f, Translation(vec3(2.0f, 1.0f, 0.0f)), base));

	// Creation d'un objet 3D (un terrain et de l'eau)

	objects.push_back(new Terrain("../tutos/multipleLights.glsl", "../data/grass.jpg", "../data/grass_spec.jpg", Identity(), base));
	//objects.push_back(new Eau("../tutos/eau2.glsl", vec3(0.0f, 0.0f, 1.0f), Identity(), base));
	Arbres * a = new Arbres("../scene/shaders/arbre.glsl", vec3(0.55f, 0.27f, 0.07f), Identity(), base, (Terrain *)objects[0]);
	for (int i = 0; i < 3; i++)
		objects.push_back(a->get_tree(i));

    // etat openGL par defaut

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre

    glClearDepth(1.f);                          // profondeur par defaut
    glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
    glEnable(GL_DEPTH_TEST);                    // activer le ztest

    return 0;   // ras, pas d'erreur
}

int Scene::render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // deplace la camera
    int mx, my;
    unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
    if (mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
        m_camera.rotation(mx, my);
    else if (mb & SDL_BUTTON(3))         // le bouton droit est enfonce
        m_camera.move(mx);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//base->ChangeTransform(   RotationZ(1));
    //objects[1]->ChangeTransform(Translation(vec3(1.0, 0.0, 0.0)));
    for(int i=0; i<objects.size(); i++){
	std:cout << "Drawing object " << i << std::endl;
        objects[i]->Draw(&m_camera, dirLight, pointLights);
    }

	return 1;
}