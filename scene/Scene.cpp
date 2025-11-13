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
	dirLight = new Dirlight(vec3(0.2, 0.2, 0.2), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), Identity()* Translation(vec3(5.0,5.0,5.0)), base, vec3(-0.2f, -1.0f, -0.1f));
	pointLights.push_back(new PointLight(vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), 1.0f, 0.09f, 0.032f, Translation(vec3(2.0f, 1.0f, 0.0f)), base));

	// LISTE DES OBJETS
	objects.push_back(new Cube("../tutos/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity(), base));
    objects.push_back(new Cube("../tutos/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity()* Translation(vec3(2.0,0.0,0.0)), base));
    objects.push_back(new Cube("../tutos/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity()* Translation(vec3(2.0,2.0,0.0)), base));
    objects.push_back(new Cube("../tutos/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity()* Translation(vec3(2.0,4.0,0.0)), base));
    //objects.push_back(new Cube("../tutos/multipleLights.glsl", vec3(0.5, 0.5, 0.5), Identity() * Translation(vec3(2.5, 0.0, 0.0)), base));
	//objects.push_back(new Terrain("../tutos/tuto9_color.glsl", vec3(0.0f,1.0f,0.0f), Identity(), base));
	//objects.push_back(new Eau("../tutos/eau2.glsl", vec3(0.0f, 0.0f, 1.0f), Identity(), base));
    objects.push_back(new Plane("../tutos/tuto9_color.glsl", vec3(0.0f, 0.0f, 1.0f), Identity() * Translation(vec3(0.0, -1.0, 0.0)), base));
	// Creation d'un objet 3D (un cube)
    objects.push_back(new Sky("../shader/sky.glsl", vec3(1.0, 1.0, 1.0), Identity(), base));
	objects.push_back(new ObjectLoad(  "../shader/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity(), base, "../data/source/van.obj" ));
    objects.push_back(new Terrain("../tutos/multipleLights.glsl", "../data/grass.jpg", "../data/grass_spec.jpg", Identity(), base));
    //objects.push_back(new ObjectLoad("../tutos/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity()* Translation(vec3(2.0,0.0,0.0)), base, "../data/source/van.obj"));
    //objects.push_back(new Cube("../tutos/tuto9_color.glsl", vec3(0.5, 0.5, 0.5), Identity() * Translation(vec3(2.5, 0.0, 0.0)), base));
	//objects.push_back(new Plane("../tutos/multipleLights.glsl","../data/container2.png","../data/container2_specular.png", Identity(), base));

    //objects.push_back(new Billboard("../shader/billboard.glsl", "../data/cloud.png", Identity() * Scale(10.), base));


	
    // etat openGL par defaut

    glClearColor(1.0f, 1.0f, 1.0f, 1.f);        // couleur par defaut de la fenetre

    glClearDepth(1.f);                          // profondeur par defaut
    glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
    glEnable(GL_DEPTH_TEST);                    // activer le ztest

    depthMapShader = read_program("../scene/shaders/depthShader.glsl"); // Shader de la depthMap

    //GLuint texSampler = glGetUniformLocation(shaderLights, "shadowMap");
    //glUniform1i(texSampler, 0);

     
    
    glGenFramebuffers(1, &m_fbo); // Creation du framebuffer

    glGenTextures(1, &m_shadowMap); // Creation du depthBuffer
    glBindTexture(GL_TEXTURE_2D, m_shadowMap); // Attacher la texture
    //glBindSampler(3, texSampler);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);

    glDrawBuffer(GL_NONE); // Pour na pas mettre de buffer de draw
    glReadBuffer(GL_NONE); // car on a besoin seulement de la profondeur

    GLuint shaderLights = read_program("../tutos/multipleLights.glsl");
    program_uniform(shaderLights,"shadowMap",0);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "ERROR : Frambuffer for DepthMap not initialized"; // Test pour verifier si le buffer est initialise
        return -1;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    

    return 0;   // ras, pas d'erreur
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void Scene::renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -5.0f,  5.0f, 0.0f, 0.0f, 1.0f,
            -5.0f, -5.0f, 0.0f, 0.0f, 0.0f,
             5.0f, 5.0f, 0.0f, 1.0f, 1.0f,
             5.0f, -5.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

Transform Scene::shadowMapPass(){

    //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // Clear du buffer

    int tps = SDL_GetTicks()/1000;
    //cout << tps << endl;

    

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

    glUseProgram(depthMapShader); // Utilisation du shader de la shadowMap

    float near_plane = 1.0f, far_plane = 50.0f; // Plans    
    Transform proj = Ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane); // Creation d'une projection orthogonale
    Transform lightView = Lookat(vec3(5.f,10.f,10.f),vec3(0.f,0.f,0.f),vec3(0.f,1.f,0.f)); // Creation d'une matrice de lumiere arbitraire : Position de la lumiere puis direction vers quoi regarde puis vecteur
    Transform model = Identity(); // Arbitraire
    mvpLight = proj * lightView * model;
    //cout << MVP[0][0] << endl;
    //cout << MVP[1][1] << endl;
    //cout << MVP[2][2] << endl;
    //cout << MVP[3][3] << endl;

    program_uniform(depthMapShader,"mvp",mvpLight);

    //glBindFramebuffer(GL_FRAMEBUFFER, 0); // Utilisation du framebuffer
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT); // Dimensions de la fenetre
    glActiveTexture(GL_TEXTURE0);

    //glBindTextureUnit(GL_TEXTURE0, m_shadowMap);
    //glUniform1i(glGetUniformLocation(shaderLights, "shadowMap"), 2);

    glBindTexture(GL_TEXTURE_2D, m_shadowMap);

    for(int i=0; i<objects.size(); i++){
        objects[i]->shadowDraw(depthMapShader, mvpLight);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    FBO_2_PPM_file();

    return mvpLight;

}

void Scene::lightingPass(){

    //glBindFramebuffer(GL_FRAMEBUFFER, 0); // Utilisation du framebuffer

    glViewport(0, 0, 1080, 720); // Dimensions de la fenetre

    for(int i=0; i<objects.size(); i++){
        //objects[i]->Draw(&m_camera, dirLight, pointLights, mvpLight,m_shadowMap);
    }


}

void Scene::FBO_2_PPM_file()
{
    FILE    *output_image;
    int     output_width, output_height;

    output_width = SHADOW_WIDTH;
    output_height = SHADOW_HEIGHT;

    /// READ THE PIXELS VALUES from FBO AND SAVE TO A .PPM FILE
    int             i, j, k;
    unsigned char   *pixels = (unsigned char*)malloc(output_width*output_height*3);

    /// READ THE CONTENT FROM THE FBO
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, output_width, output_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    output_image = fopen("C:\\Dataset\\output.ppm", "wt");
    fprintf(output_image,"P3\n");
    fprintf(output_image,"# Created by Ricao\n");
    fprintf(output_image,"%d %d\n",output_width,output_height);
    fprintf(output_image,"255\n");

    k = 0;
    for(i=0; i<output_width; i++)
    {
        for(j=0; j<output_height; j++)
        {
            fprintf(output_image,"%u %u %u ",(unsigned int)pixels[k],(unsigned int)pixels[k+1],
                                             (unsigned int)pixels[k+2]);
            k = k+3;
        }
        fprintf(output_image,"\n");
    }
    free(pixels);
}


int Scene::render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Transform mvpLight = shadowMapPass();
    lightingPass();

    // deplace la camera
    int mx, my;
    unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
    if (mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
        m_camera.rotation(mx, my);
    else if (mb & SDL_BUTTON(3))         // le bouton droit est enfonce
        m_camera.move(mx);



    Transform view = m_camera.view();
    Transform camWorld = view.inverse(); // pour repasser dans l�espace monde

    // vecteurs cam�ra en espace monde
    Vector forward = normalize(camWorld(Vector(0.0f, 0.0f, -1.0f)));
    Vector right = normalize(camWorld(Vector(1.0f, 0.0f, 0.0f)));

    const float moveSpeed = 0.1f;
    const float scaleSpeed = 0.02f;

    // --- �CHELLE ---
    // Z = agrandir, S = r�tr�cir
    if (key_state(SDLK_z))
        base->ChangeTransform(Scale(1.0f + scaleSpeed));
    else if (key_state(SDLK_s))
        base->ChangeTransform(Scale(1.0f - scaleSpeed));

    // --- TRANSLATION (selon la cam�ra) ---
    // Q = gauche, D = droite
    if (key_state(SDLK_q))
        base->ChangeTransform(Translation(right * moveSpeed));
    else if (key_state(SDLK_d))
        base->ChangeTransform(Translation(-right * moveSpeed));



    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Utilisation du framebuffer

	//base->ChangeTransform(   RotationZ(1));
    //objects[0]->ChangeTransform(RotationY(10));
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f; // en secondes
    lastTime = currentTime;

	dirLight->Rotation(deltaTime *30);
	//std::cout << dirLight->direction.x << " " << dirLight->direction.y << " " << dirLight->direction.z << std::endl;

	//objects[2]->ChangeTransform(Translation(vec3(1.0 * deltaTime , 0.0 , 0.0)));
    for(int i=0; i<objects.size(); i++){
        //objects[i]->Draw(&m_camera, dirLight, pointLights, mvpLight,m_shadowMap);
        objects[i]->Draw(&m_camera, dirLight, pointLights);
    }

    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    renderQuad();

	return 1;
}