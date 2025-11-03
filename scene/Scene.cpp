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
	dirLight = new Dirlight(vec3(0.2, 0.2, 0.2), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), Identity()* Translation(vec3(5.0,5.0,5.0)), base, vec3(0.2f, 1.0f, 0.5f));
	pointLights.push_back(new PointLight(vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), 1.0f, 0.09f, 0.032f, Translation(vec3(2.0f, 1.0f, 0.0f)), base));

	// Creation d'un objet 3D (un cube)

	objects.push_back(new ObjectLoad(  "../shader/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity(), base, "../data/source/van.obj" ));
    //objects.push_back(new ObjectLoad("../tutos/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity()* Translation(vec3(2.0,0.0,0.0)), base, "../data/source/van.obj"));
    //objects.push_back(new Cube("../tutos/tuto9_color.glsl", vec3(0.5, 0.5, 0.5), Identity() * Translation(vec3(2.5, 0.0, 0.0)), base));
	//objects.push_back(new Plane("../tutos/multipleLights.glsl","../data/container2.png","../data/container2_specular.png", Identity(), base));
	//objects.push_back(new Billboard("../shader/billboard.glsl", "../data/cloud.png", Identity(), base));
	objects.push_back(new Sky("../shader/sky.glsl", vec3(1.0,1.0,1.0), Identity() * Scale(100.0), base));
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
    //if (mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
    //    m_camera.rotation(mx, my);
    //else if (mb & SDL_BUTTON(3))         // le bouton droit est enfonce
    //    m_camera.move(mx);
    if (mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
		base->ChangeTransform(RotationY(-mx) * RotationX(-my));



    Transform view = m_camera.view();
    Transform camWorld = view.inverse(); // pour repasser dans l’espace monde

    // vecteurs caméra en espace monde
    Vector forward = normalize(camWorld(Vector(0.0f, 0.0f, -1.0f)));
    Vector right = normalize(camWorld(Vector(1.0f, 0.0f, 0.0f)));

    const float moveSpeed = 0.1f;
    const float scaleSpeed = 0.02f;

    // --- ÉCHELLE ---
    // Z = agrandir, S = rétrécir
    if (key_state(SDLK_z))
        base->ChangeTransform(Scale(1.0f + scaleSpeed));
    else if (key_state(SDLK_s))
        base->ChangeTransform(Scale(1.0f - scaleSpeed));

    // --- TRANSLATION (selon la caméra) ---
    // Q = gauche, D = droite
    if (key_state(SDLK_q))
        base->ChangeTransform(Translation(right * moveSpeed));
    else if (key_state(SDLK_d))
        base->ChangeTransform(Translation(-right * moveSpeed));



    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//base->ChangeTransform(   RotationZ(1));
    //objects[0]->ChangeTransform(RotationY(10));
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f; // en secondes
    lastTime = currentTime;

	dirLight->Rotation(deltaTime * 0.1);
    for(int i=0; i<objects.size(); i++){
        objects[i]->Draw(&m_camera, dirLight, pointLights);
    }

	return 1;
}