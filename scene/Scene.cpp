#include "Scene.h"

Scene::Scene(){
}
Scene::~Scene(){
    for(int i=0; i<objects.size(); i++){
        delete objects.front();
        objects.pop_front();
	}
    for(int i=0; i<lights.size(); i++){
        delete lights.front();
        lights.pop_front();
	}
}
int Scene::Init(){

	base = new Entity();
	// Creation d'une lumiere

	// Creation d'un objet 3D (un cube)

    // etat openGL par defaut
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre

    glClearDepth(1.f);                          // profondeur par defaut
    glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
    glEnable(GL_DEPTH_TEST);                    // activer le ztest

    return 0;   // ras, pas d'erreur
}

void Scene::Render(){
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
}