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

	// LISTE DES OBJETS
	objects.push_back(new Cube("../tutos/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity(), base));
    objects.push_back(new Cube("../tutos/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity()* Translation(vec3(2.0,0.0,0.0)), base));
    //objects.push_back(new Cube("../tutos/tuto9_color.glsl", vec3(0.5, 0.5, 0.5), Identity() * Translation(vec3(2.5, 0.0, 0.0)), base));
	//objects.push_back(new Terrain("../tutos/tuto9_color.glsl", vec3(0.0f,1.0f,0.0f), Identity(), base));
	//objects.push_back(new Eau("../tutos/eau2.glsl", vec3(0.0f, 0.0f, 1.0f), Identity(), base));
    objects.push_back(new Plane("../tutos/tuto9_color.glsl", vec3(0.0f, 0.0f, 1.0f), Identity() * Translation(vec3(0.0, -1.0, 0.0)), base));
    // etat openGL par defaut

    glClearColor(1.0f, 1.0f, 1.0f, 1.f);        // couleur par defaut de la fenetre

    glClearDepth(1.f);                          // profondeur par defaut
    glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
    glEnable(GL_DEPTH_TEST);                    // activer le ztest

    depthMapShader = read_program("../scene/shaders/depthShader.glsl"); // Shader de la depthMap

    glGenFramebuffers(1, &m_fbo); // Creation du framebuffer

    glGenTextures(1, &m_shadowMap); // Creation du depthBuffer
    glBindTexture(GL_TEXTURE_2D, m_shadowMap); // Attacher la texture 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);

    glDrawBuffer(GL_NONE); // Pour na pas mettre de buffer de draw
    glReadBuffer(GL_NONE); // car on a besoin seulement de la profondeur
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "ERROR : Frambuffer for DepthMap not initialized"; // Test pour verifier si le buffer est initialise
        return -1;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return 0;   // ras, pas d'erreur
}

void Scene::shadowMapPass(){

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

    glClear(GL_DEPTH_BUFFER_BIT); // Clear du buffer

    glUseProgram(depthMapShader); // Utilisation du shader de la shadowMap

    float near_plane = 1.0f, far_plane = 50.0f; // Plans
    //glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane); // Creation d'une projection orthogonale
    //glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3( 0.0f, 0.0f,  0.0f), glm::vec3( 0.0f, 1.0f,  0.0f)); // Creation d'une matrice de lumiere arbitraire : Position de la lumiere puis direction vers quoi regarde puis vecteur
    //glm::mat4 depthModelMatrix = glm::mat4(1.0); // Arbitraire
    //glm::mat4 MVP = depthModelMatrix * lightProjection * lightView; // Matrice a donner au shader
    //Transform mvp2 = Transform(MVP[0][0],MVP[0][1],MVP[0][2],MVP[0][3],MVP[1][0],MVP[1][1],MVP[1][2],MVP[1][3],MVP[2][0],MVP[2][1],MVP[2][2],MVP[2][3],MVP[3][0],MVP[3][1],MVP[3][2],MVP[3][3]);
    
    Transform proj = Ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane); // Creation d'une projection orthogonale
    Transform lightView = Lookat(vec3(20.f,4.f,4.f),vec3(0.f,0.f,0.f),vec3(0.f,1.f,0.f)); // Creation d'une matrice de lumiere arbitraire : Position de la lumiere puis direction vers quoi regarde puis vecteur
    Transform model = Identity(); // Arbitraire
    mvpLight = proj * lightView * model;
    //cout << MVP[0][0] << endl;
    //cout << MVP[1][1] << endl;
    //cout << MVP[2][2] << endl;
    //cout << MVP[3][3] << endl;

    program_uniform(depthMapShader,"mvp",mvpLight);

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Utilisation du framebuffer
    glViewport(0, 0, 1080, 720); // Dimensions de la fenetre
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i=0; i<objects.size(); i++){
        //objects[i]->shadowDraw(depthMapShader, mvpLight);
    }

}

void Scene::lightingPass(){

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Utilisation du framebuffer

    glViewport(0, 0, 1080, 720); // Dimensions de la fenetre

    //shadow.bindForReading(GL_TEXTURE1);


    for(int i=0; i<objects.size(); i++){
        objects[i]->Draw(&m_camera, dirLight, pointLights);
    }


}

int Scene::render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shadowMapPass();
    lightingPass();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);  

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
        objects[i]->Draw(&m_camera, dirLight, pointLights);
    }


	return 1;
}