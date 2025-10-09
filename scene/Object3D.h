#ifndef _3DOBJECT
#define _3DOBJECT

#include "Entity.h"
#include "mat.h"
#include "mesh.h"
#include "wavefront.h"
#include "texture.h"

#include "program.h"
#include "uniforms.h"
#include "draw.h"

#include "lights/PointLight.h"
#include "lights/DirLight.h"
#include <string>

/*
    @class Object3D
    Classe representant un objet 3D
*/

class Object3D : public Entity {
    
    protected:
        Mesh mesh; ///Mesh de l'objet
		GLuint texture; ///Texture de l'objet
		GLuint texture_specular; ///Seconde texture de l'objet
		GLuint shader; ///Shader de l'objet
		vec3 color; ///Couleur de l'objet

    public:
		Object3D(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
		Object3D(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
		Object3D(std::string strShader, vec3 c, Transform tr, Entity* p); ///Constructeur couleur
		~Object3D(); ///Destructeur

		void Draw(Orbiter * camera, Dirlight * dirLight, vector<PointLight*> pointLights); ///Dessine l'objet

};

#endif