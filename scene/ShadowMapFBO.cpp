#include "ShadowMapFBO.h"

bool ShadowMapFBO::init(unsigned int width, unsigned int height){

    m_width = width;
    m_height = height;

    glGenFramebuffers(1, &m_fbo); // Creation du framebuffer

    glGenTextures(1, &m_shadowMap); // Creation du depthBuffer
    glBindTexture(GL_TEXTURE_2D, m_shadowMap); // Attacher la texture 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);

    glDrawBuffer(GL_NONE); // Pour na pas mettre de buffer de draw
    glReadBuffer(GL_NONE); // car on a besoin seulement de la profondeur
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "ERROR : Frambuffer for DepthMap not initialized"; // Test pour verifier si le buffer est initialise
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}