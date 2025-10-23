#ifndef _SKY
#define _SKY

#include "Object3D.h"

/*
    @class Light
    Classe representant une lumiere
*/

class Sky : public Object3D {
    
    public:
		vector<GLuint> skyTextures; 

        Sky(std::string strShader, std::string strTexture1, Transform tr, Entity* p); ///Constructeur une seule texture
        Sky(std::string strShader, std::string strTexture1, std::string strTexture2, Transform tr, Entity* p); ///Constructeur deux textures
        Sky(std::string strShader, vec3 c, Transform tr, Entity* p); ///Constructeur couleur


        void Draw(Orbiter* camera, Dirlight* dirLight, vector<PointLight*> pointLights);
};

#endif