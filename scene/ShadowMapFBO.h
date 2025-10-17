#ifndef _SHADOWMAPFBO
#define _SHADOWMAPFBO

#include <glew.h>
#include <iostream>

/*
    @class ShadowMapFBO
    Classe representant une ShadowMapFBO
*/

class ShadowMapFBO{

    public:
        ShadowMapFBO();
        bool init(unsigned int width, unsigned int height);
        void bindForWriting();
        void bindForReading(GLenum textureUnit);

        uint m_width = 0;
        uint m_height = 0;
        GLuint m_fbo;
        GLuint m_shadowMap;

};

#endif