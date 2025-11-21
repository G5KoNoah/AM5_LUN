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

    public:
		Scene() : App(1024, 640) {} ///Constructeur
        int quit(); ///Destructeur
		int init(); ///Initialise la scene
		int render() override; ///Dessine la scene
        Transform shadowMapPass();
        void lightingPass();
        void renderQuad();
        void FBO_2_PPM_file(string st,int width, int height);

        float scaleFactor = 1.0f;
        float prevScale = 1.0f;
		// pour appliquer des translations relatives (delta)
		Vector prevTranslation = Vector(0.0f, 0.0f, 0.0f);
		Vector currentTranslation = Vector(0.0f, 0.0f, 0.0f);

        float waterHeight = 1.0; // Hauteur de l'eau

        // Framebuffers d'eau
        int createFrameBuffer();
        int createTextureAttachment( int width, int height);
        int createDepthTextureAttachment( int width, int height);
        int createDepthBufferAttachment(int width, int height);

        void initialiseReflectionFrameBuffer();
        void initialiseRefractionFrameBuffer();

        void bindFrameBuffer(int frameBuffer, int width, int height);
        void showFramebufferError();

        GLuint m_fbo;
        GLuint m_shadowMap;
        GLuint depthMapShader; // Shader de la depthMap

        unsigned int reflectionFrameBuffer;
	    unsigned int reflectionTexture;
	    unsigned int reflectionDepthBuffer;

        unsigned int refractionFrameBuffer;
	    unsigned int refractionTexture;
	    unsigned int refractionDepthTexture;

        const unsigned int SHADOW_WIDTH = 1024;
        const unsigned int SHADOW_HEIGHT = 720;

        static const int REFLECTION_WIDTH = 320;
        static const int REFLECTION_HEIGHT = 180;

        static const int REFRACTION_WIDTH = 320;
        static const int REFRACTION_HEIGHT = 180;

        //Geters
        GLuint getReflectionTexture();
        GLuint getRefractionTexture();
        GLuint getRefractionDepthTexture();

        //Bind
        void bindReflectionFrameBuffer();
        void bindRefractionFrameBuffer();

        //Unbind
        void unbindCurrentFrameBuffer();

        //Nettoyage de l'eau
        void waterCleanUp();

        //Affichage dans texture
        void renderWater();

        
};

#endif