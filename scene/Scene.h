#ifndef _SCENE
#define _SCENE

#include "Object3D.h"
#include "lights/DirLight.h"
#include "lights/PointLight.h"
#include "Cube.h"
#include "terrain.h"
#include "Plane.h"
#include "Eau.h"

#include "mat.h"
#include "mesh.h"
#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "draw.h"

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
        Transform mvpLight;

    public:
		Scene() : App(1080, 720) {} ///Constructeur
        int quit(); ///Destructeur
		int init(); ///Initialise la scene
		int render() override; ///Dessine la scene
        void shadowMapPass();
        void lightingPass();

        GLuint m_fbo;
        GLuint m_shadowMap;
        GLuint depthMapShader; // Shader de la depthMap

        const unsigned int SHADOW_WIDTH = 1024;
        const unsigned int SHADOW_HEIGHT = 1024;
        
};

#endif