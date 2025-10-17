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

	// Creation d'un objet 3D (un terrain et un plane)

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

    shadow.init(SHADOW_WIDTH,SHADOW_HEIGHT); // Initialisation du Framebuffer de la depthMap

    return 0;   // ras, pas d'erreur
}

void Scene::shadowMapPass(){

    shadow.bindForWriting(); // Activation du Framebuffer de la shadowMap

    glClear(GL_DEPTH_BUFFER_BIT); // Clear du buffer

    glUseProgram(depthMapShader); // Utilisation du shader de la shadowMap

    float near_plane = 1.0f, far_plane = 7.5f; // Plans
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane); // Creation d'une projection orthogonale
    glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3( 0.0f, 0.0f,  0.0f), glm::vec3( 0.0f, 1.0f,  0.0f)); // Creation d'une matrice de lumiere arbitraire : Position de la lumiere puis direction vers quoi regarde puis vecteur
    glm::mat4 depthModelMatrix = glm::mat4(1.0); // Arbitraire
    glm::mat4 MVP = depthModelMatrix * lightProjection * lightView; // Matrice a donner au shader
    Transform mvp = Transform(MVP[0][0],MVP[0][1],MVP[0][2],MVP[0][3],MVP[1][0],MVP[1][1],MVP[1][2],MVP[1][3],MVP[2][0],MVP[2][1],MVP[2][2],MVP[2][3],MVP[3][0],MVP[3][1],MVP[3][2],MVP[3][3]);
    
    program_uniform(depthMapShader,"mvp",mvp);

    objects[0]->shadowDraw(depthMapShader,mvp);

}

int Scene::render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //=====================================
    //     TEST POUR SHADOW MAPPING
    //=====================================

    //shadowMapPass();

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
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane); // Creation d'une projection orthogonale
    glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3( 0.0f, 0.0f,  0.0f), glm::vec3( 0.0f, 1.0f,  0.0f)); // Creation d'une matrice de lumiere arbitraire : Position de la lumiere puis direction vers quoi regarde puis vecteur
    glm::mat4 depthModelMatrix = glm::mat4(1.0); // Arbitraire
    glm::mat4 MVP = depthModelMatrix * lightProjection * lightView; // Matrice a donner au shader
    Transform mvp = Transform(MVP[0][0],MVP[0][1],MVP[0][2],MVP[0][3],MVP[1][0],MVP[1][1],MVP[1][2],MVP[1][3],MVP[2][0],MVP[2][1],MVP[2][2],MVP[2][3],MVP[3][0],MVP[3][1],MVP[3][2],MVP[3][3]);
    
    glUseProgram(depthMapShader);

    glUniformMatrix4fv(1,1,false,glm::value_ptr(MVP));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    //FAIRE LE RENDU DE LA SCENE AVEC LE SHADER;
    for(int i=0; i<objects.size(); i++){
        objects[i]->shadowDraw(depthMapShader, mvp);
    }    
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