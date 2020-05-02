#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>
#include <stdlib.h>

// Shadow Mapping
GLuint shadow_fbo;
GLuint shadow_tex;
GLuint shadow_depth;

//TODO
inline void shadowInitialize(int frameBufferWidth, int frameBufferHeight, int unit) {
    glGenFramebuffers(1, &shadow_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo);

    glGenTextures(1, &shadow_tex);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, shadow_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, frameBufferWidth, frameBufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_tex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, unit);

//    glGenFramebuffers(1, &shadow_fbo);
//    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo);
//
//    // Create the texture object for the primitive information buffer
//    glGenTextures(1, &shadow_tex);
//    glActiveTexture(GL_TEXTURE0 + unit);
//    glBindTexture(GL_TEXTURE_2D, shadow_tex);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, frameBufferWidth, frameBufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
//
//    // Attach to primitive texture image to a framebuffer object
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadow_tex, 0);
//
//    glGenRenderbuffers(1, &shadow_depth);
//    glBindRenderbuffer(GL_RENDERBUFFER, shadow_depth);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frameBufferWidth, frameBufferHeight);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, shadow_depth);
//
//    glReadBuffer(GL_NONE);
//    glDrawBuffer(GL_NONE);
//
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//        std::cout << "ERROR: Framebuffer is not complete" << std::endl;
//        std::cin.get();
//        std::terminate();
//    }
//
//    // Unbind this framebuffer
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, unit);
}

inline void reallocateShadowTexture(int frameBufferWidth, int frameBufferHeight, int unit) {
    glDeleteTextures(1, &shadow_tex);
    glDeleteTextures(1, &shadow_depth);
    shadowInitialize(frameBufferWidth, frameBufferHeight, unit);
}

inline void deleteShadowResources() {
    glDeleteTextures(1, &shadow_tex);
    glDeleteRenderbuffers(1, &shadow_depth);
    glDeleteFramebuffers(1, &shadow_fbo);
}