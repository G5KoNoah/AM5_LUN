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
	dirLight = new Dirlight(vec3(0.2, 0.2, 0.2), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), Identity()* Translation(vec3(5.0,5.0,5.0)), base, vec3(-0.2f, -1.0f, -0.3f));
	//pointLights.push_back(new PointLight(vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), 1.0f, 0.09f, 0.032f, Translation(vec3(2.0f, 1.0f, 0.0f)), base));

	// Creation d'un objet 3D (un cube)

	objects.push_back(new Cube("../tutos/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity(), base));
    objects.push_back(new Cube("../tutos/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity()* Translation(vec3(2.0,0.0,0.0)), base));
    objects.push_back(new Cube("../tutos/tuto9_color.glsl", vec3(0.5, 0.5, 0.5), Identity() * Translation(vec3(2.5, 0.0, 0.0)), objects[1]));
    // etat openGL par defaut

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre

    glClearDepth(1.f);                          // profondeur par defaut
    glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
    glEnable(GL_DEPTH_TEST);                    // activer le ztest

    



    return 0;   // ras, pas d'erreur
}

int Scene::render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //=====================================
    //     TEST POUR SHADOW MAPPING
    //=====================================
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO); //Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    // Texture de la shadowmap
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap); // Attacher la texture 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE); // Pour na pas mettre de buffer de draw
    glReadBuffer(GL_NONE); // car on a besoin seulement de la profondeur
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        cout << "ERROR : Frambuffer for DepthMap not initialized"; // Test pour verifier si le buffer est initialise
    }

    /// Framebuffer complet

    // Faire le rendu de la shadowmap
    // Idee est de faire le rendu de la scene du point de vue de la lumiere et de la stocker dans une texture
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);


    float near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

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
    objects[1]->ChangeTransform(Translation(vec3(1.0, 0.0, 0.0)));
    for(int i=0; i<objects.size(); i++){
        objects[i]->Draw(&m_camera, dirLight, pointLights);
    }

	return 1;
}