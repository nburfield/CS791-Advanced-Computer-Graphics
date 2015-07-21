/*
  Used as the geometry pass for the Deferred Shading
*/

#ifndef GEOM_PASS_H
#define GEOM_PASS_H

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  #include <GL/glu.h>
#endif

#include <iostream>
#include <programHandle.h>

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GeomPass : public Program
{
  public:
    GeomPass();
    ~GeomPass();
    bool Init();
    void SetMVP(glm::mat4 wvp);
    void SetModel(glm::mat4 Model);
    void SetTexture(unsigned int TextureUnit);

  private:
    GLuint mvp;
    GLuint model;
    GLuint texture;
};

#endif
