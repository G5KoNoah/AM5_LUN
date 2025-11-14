#include "WaterFBO.h"

/*

void WaterFBO::initialiseReflectionFrameBuffer() {
	reflectionFrameBuffer = createFrameBuffer();
	reflectionTexture = createTextureAttachment(REFLECTION_WIDTH,REFLECTION_HEIGHT);
	reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH,REFLECTION_HEIGHT);	
    unbindCurrentFrameBuffer();
}


void WaterFBO::initialiseRefractionFrameBuffer()
{
    refractionFrameBuffer = createFrameBuffer();
	refractionTexture = createTextureAttachment(REFRACTION_WIDTH,REFRACTION_HEIGHT);
	refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH,REFRACTION_HEIGHT);
	unbindCurrentFrameBuffer();
}

WaterFBO::WaterFBO() { // Whan starting the programm
	initialiseReflectionFrameBuffer();
	initialiseRefractionFrameBuffer();
}

void WaterFBO::bindReflectionFrameBuffer()
{
    bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void WaterFBO::bindRefractionFrameBuffer()
{
    bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFBO::unbindCurrentFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //back to default frame buffer
	glViewport(0, 0, 1600, 1200);
}

void WaterFBO::cleanUp()
{
    glDeleteFramebuffers(1, &reflectionFrameBuffer);
    glDeleteTextures(1, &reflectionTexture);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer);
    glDeleteFramebuffers(1, &refractionFrameBuffer);
    glDeleteTextures(1, &refractionTexture);
    glDeleteTextures(1, &refractionDepthTexture);
}


int WaterFBO::createDepthBufferAttachment(int width, int height)
{
        unsigned int depthBuffer;
        glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		return depthBuffer;
}

void WaterFBO::bindFrameBuffer(int frameBuffer, int width, int height){
    //glBindTexture(GL_TEXTURE_2D, 0); //To make sure the texture isn't bound

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl; 
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
}

unsigned int WaterFBO::getReflectionTexture() {
    return reflectionTexture;
}

unsigned int WaterFBO::getRefractionTexture() {
    return refractionTexture;
}

unsigned int WaterFBO::getRefractionDepthTexture() {
    return refractionDepthTexture;
}

*/