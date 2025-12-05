#ifndef _WATERFBO
#define _WATERFBO

#include <../GLFW/glfw3.h>
#include <../GL/glew.h>
#include "app.h"


class WaterFBO{

    static int windowWidth;
    static int windowHeight;
    
    static const int REFLECTION_WIDTH = 320;
    static const int REFLECTION_HEIGHT = 180;

    static const int REFRACTION_WIDTH = 320;
    static const int REFRACTION_HEIGHT = 180;

    unsigned int reflectionFrameBuffer;
	unsigned int reflectionTexture;
	unsigned int reflectionDepthBuffer;

    unsigned int refractionFrameBuffer;
	unsigned int refractionTexture;
	unsigned int refractionDepthTexture;

    int createFrameBuffer();
    int createTextureAttachment( int width, int height);
    int createDepthTextureAttachment( int width, int height);
    int createDepthBufferAttachment(int width, int height);

    void initialiseReflectionFrameBuffer();
    void initialiseRefractionFrameBuffer();

    void bindFrameBuffer(int frameBuffer, int width, int height);

    public:
    WaterFBO();
    //Geters
    GLuint getReflectionTexture();
    GLuint getRefractionTexture();
    GLuint getRefractionDepthTexture();

    //Bind
    void bindReflectionFrameBuffer();
    void bindRefractionFrameBuffer();

    //Unbind
    void unbindCurrentFrameBuffer();

    //cleanup
    void cleanUp();

};

#endif