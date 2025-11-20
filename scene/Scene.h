#ifndef _SCENE
#define _SCENE

#include "Object3D.h"
#include "ObjectLoad.h"
#include "Plane.h"
#include "Billboard.h"
#include "lights/DirLight.h"
#include "lights/PointLight.h"
#include "Cube.h"
#include "Terrain.h"
#include "Plane.h"
#include "Eau.h"
#include "Arbres.h"
#include "Sky.h"

#include "mat.h"
#include "mesh.h"
#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "draw.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstdio>
#include <string>


#include "app.h"        // classe Application a deriver
/*
    @class Scene
    Classe representant une scene
*/

class Scene : public App{

    private:
		Entity * base; ///Racine de la scene
        vector<Object3D*> objects; ///Liste des objets de la scene
		vector<PointLight*> pointLights; ///Liste des lumieres ponctuelles de la scene
		Dirlight* dirLight = nullptr; ///Lumiere directionnelle de la scene
        Orbiter m_camera;
        Uint32 lastTime = SDL_GetTicks();
        Transform mvpLight;

		float scaleFactor = 1.0f;
        float prevScale = 1.0f;
		// pour appliquer des translations relatives (delta)
		Vector prevTranslation = Vector(0.0f, 0.0f, 0.0f);
		Vector currentTranslation = Vector(0.0f, 0.0f, 0.0f);

    public:
		Scene() : App(1024, 640) {} ///Constructeur
        int quit(); ///Destructeur
		int init(); ///Initialise la scene
		int render() override; ///Dessine la scene
        Transform shadowMapPass();
        void lightingPass();
        void renderQuad();
        void FBO_2_PPM_file();

        GLuint m_fbo;
        GLuint m_shadowMap;
        GLuint depthMapShader; // Shader de la depthMap

        const unsigned int SHADOW_WIDTH = 1024;
        const unsigned int SHADOW_HEIGHT = 720;
        
};

#endif