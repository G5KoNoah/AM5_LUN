#include "Scene.h"


int Scene::quit(){
    for(int i=0; i<objects.size(); i++){
		delete objects[i];
	}
	objects.clear();
    waterCleanUp();
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
    //objects.push_back(new Plane("../tutos/tuto9_color.glsl", vec3(0.0f, 0.0f, 1.0f), Identity() * Translation(vec3(0.0, -1.0, 0.0)), base));
    objects.push_back(new Sky("../shader/sky.glsl", vec3(1.0, 1.0, 1.0), Identity(), base));
	objects.push_back(new ObjectLoad(  "../shader/multipleLights.glsl", "../data/textures/Material_BaseColor.png", "../data/textures/Material_Metallic.png", Identity(), base, "../data/source/van.obj" ));
    objects.push_back(new Terrain("../tutos/multipleLights.glsl", "../data/grass.jpg", "../data/grass_spec.jpg", Identity(), base));
    objects.push_back(new Eau("../scene/shaders/eau2.glsl", vec3(0.0f, 0.0f, 1.0f), Identity()* Translation(vec3(-20.0,waterHeight,-20.0)), base));
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

    // Initialisation des framebuffers d'eau
    initialiseReflectionFrameBuffer();
	initialiseRefractionFrameBuffer();

     
    
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
    Transform lightView = Lookat(vec3(10.f,10.f,-8.f),vec3(0.f,0.f,0.f),vec3(0.f,1.f,0.f)); // Creation d'une matrice de lumiere arbitraire : Position de la lumiere puis direction vers quoi regarde puis vecteur
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
    
    //FBO_2_PPM_file("ShadowMap.ppm");

    return mvpLight;

}

void Scene::lightingPass(){

    //glBindFramebuffer(GL_FRAMEBUFFER, 0); // Utilisation du framebuffer

    glViewport(0, 0, 1080, 720); // Dimensions de la fenetre

    for(int i=0; i<objects.size(); i++){
        //objects[i]->Draw(&m_camera, dirLight, pointLights, mvpLight,m_shadowMap);
    }


}

void Scene::FBO_2_PPM_file(string st,int width, int height)
{
    FILE    *output_image;
    int     output_width, output_height;

    output_width = width;
    output_height = height;

    /// READ THE PIXELS VALUES from FBO AND SAVE TO A .PPM FILE
    int             i, j, k;
    unsigned char   *pixels = (unsigned char*)malloc(output_width*output_height*3);

    /// READ THE CONTENT FROM THE FBO
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, output_width, output_height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    output_image = fopen(st.c_str(), "wt");
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
    Vector forward = normalize(camWorld(Vector(0.0f, 0.0f, 1.0f)));
    Vector right = normalize(camWorld(Vector(1.0f, 0.0f, 0.0f)));

    const float moveSpeed = 0.1f;
    const float scaleSpeed = 0.02f;

    const float scaleStep = 0.01f;
    const float minScale = 0.001f;

    if (key_state(SDLK_r)) {
        float newScale = scaleFactor + scaleStep;
        float delta = newScale / prevScale;
        base->ChangeTransform(Scale(delta)); // applique seulement la variation
        prevScale = newScale;
        scaleFactor = newScale;
    }
    else if (key_state(SDLK_f)) {
        float newScale = scaleFactor - scaleStep;
        if (newScale < minScale) newScale = minScale;
        float delta = newScale / prevScale;
        base->ChangeTransform(Scale(delta));
        prevScale = newScale;
        scaleFactor = newScale;
    }

    // --- TRANSLATION (selon la caméra) ---
    // Q = gauche, D = droite
    // calculer la nouvelle translation voulue en espace monde (accumulée)
    Vector newTranslation = currentTranslation;
    if (key_state(SDLK_q)) {
        Vector deltaMove = right * moveSpeed; // right est Vector
        newTranslation = Vector(currentTranslation.x + deltaMove.x,
                                currentTranslation.y + deltaMove.y,
                                currentTranslation.z + deltaMove.z);
    }
    else if (key_state(SDLK_d)) {
        Vector deltaMove = right * moveSpeed;
        newTranslation = Vector(currentTranslation.x - deltaMove.x,
                                currentTranslation.y - deltaMove.y,
                                currentTranslation.z - deltaMove.z);
    }
    else if (key_state(SDLK_z)) {
        Vector deltaMove = forward * moveSpeed;
        newTranslation = Vector(currentTranslation.x + deltaMove.x,
            currentTranslation.y + deltaMove.y,
            currentTranslation.z + deltaMove.z);
    }else if (key_state(SDLK_s)) {
        Vector deltaMove = forward * moveSpeed;
        newTranslation = Vector(currentTranslation.x - deltaMove.x,
            currentTranslation.y - deltaMove.y,
            currentTranslation.z - deltaMove.z);
    }
    // n'appliquer que la différence par rapport à la translation précédente
    Vector delta = Vector(newTranslation.x - prevTranslation.x,
                          newTranslation.y - prevTranslation.y,
                          newTranslation.z - prevTranslation.z);

    if (delta.x != 0.0f || delta.y != 0.0f || delta.z != 0.0f) {
        base->ChangeTransform(Translation(delta));
        prevTranslation = newTranslation;
        currentTranslation = newTranslation;
    }

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

    

    glEnable(GL_CLIP_DISTANCE0); // Activation de gl_clip_distance

    bindReflectionFrameBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float distance = 2 * (m_camera.position().y - waterHeight); // Renverser la camera pour illusion de reflections dans l'eau
    //TODO
    for(int i=0; i<objects.size(); i++){
        objects[i]->Draw(&m_camera, dirLight, pointLights,waterHeight,true);
        //FBO_2_PPM_file("ReflectionFramebuffer.ppm",REFLECTION_WIDTH,REFLECTION_HEIGHT);
    }
    unbindCurrentFrameBuffer();

    bindRefractionFrameBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i=0; i<objects.size(); i++){
        objects[i]->Draw(&m_camera, dirLight, pointLights,waterHeight,false);
        //FBO_2_PPM_file("RefractionFramebuffer.ppm",REFRACTION_WIDTH,REFRACTION_HEIGHT);
    }
    unbindCurrentFrameBuffer();

    

    glDisable(GL_CLIP_DISTANCE0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i=0; i<objects.size(); i++){
        objects[i]->Draw(&m_camera, dirLight, pointLights);
        //objects[i]->Draw(&m_camera, dirLight, pointLights,waterHeight,true);
        FBO_2_PPM_file("Frammebuffer",1024,640);
    }

    //glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    //renderQuad();

	return 1;
}






///
/// WATER FBO
///

void Scene::initialiseReflectionFrameBuffer() {
    cout << "Scene : Initialisation du FBO de reflection" << endl;
	reflectionFrameBuffer = createFrameBuffer();
	reflectionTexture = createTextureAttachment(REFLECTION_WIDTH,REFLECTION_HEIGHT);
	reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH,REFLECTION_HEIGHT);	
    unbindCurrentFrameBuffer();
    cout << "Scene : FBO de reflection initialise" << endl;
}


void Scene::initialiseRefractionFrameBuffer()
{
    cout << "Scene : Initialisation du FBO de refraction" << endl;
    refractionFrameBuffer = createFrameBuffer();
	refractionTexture = createTextureAttachment(REFRACTION_WIDTH,REFRACTION_HEIGHT);
	refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH,REFRACTION_HEIGHT);
	unbindCurrentFrameBuffer();
    cout << "Scene : FBO de reflection initalise" << endl;
}

void Scene::bindReflectionFrameBuffer()
{
    bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void Scene::bindRefractionFrameBuffer()
{
    bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void Scene::unbindCurrentFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //back to default frame buffer
	glViewport(0, 0, 1024, 640);
    cout << "Scene : FBO de base initalise" << endl;
}

void Scene::waterCleanUp()
{
    glDeleteFramebuffers(1, &reflectionFrameBuffer);
    glDeleteTextures(1, &reflectionTexture);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer);
    glDeleteFramebuffers(1, &refractionFrameBuffer);
    glDeleteTextures(1, &refractionTexture);
    glDeleteTextures(1, &refractionDepthTexture);
}


int Scene::createFrameBuffer()
{
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo); 
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    return fbo;
}

int Scene::createTextureAttachment(int width, int height)
{
	unsigned int texture;
    glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	return texture;
}

int Scene::createDepthTextureAttachment(int width, int height)
{
    unsigned int texture;
    glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
    return texture;
}

int Scene::createDepthBufferAttachment(int width, int height)
{
    unsigned int depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
}

void Scene::bindFrameBuffer(int frameBuffer, int width, int height){
    //glBindTexture(GL_TEXTURE_2D, 0); //To make sure the texture isn't bound

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl; 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glViewport(0, 0, width, height);
    cout << "Scene : Changement de framebuffer : " << frameBuffer << endl;
}

unsigned int Scene::getReflectionTexture() {
    return reflectionTexture;
}

unsigned int Scene::getRefractionTexture() {
    return refractionTexture;
}

unsigned int Scene::getRefractionDepthTexture() {
    return refractionDepthTexture;
}