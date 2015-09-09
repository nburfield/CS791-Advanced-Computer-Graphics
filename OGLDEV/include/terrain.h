#ifndef TERRAIN_H
#define TERRAIN_H

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  #include <GL/glu.h>
#endif

#include <iostream>
#include "technique.h"
#include "ogldev_math_3d.h"
#include <map>
#include <vector>
#include "ogldev_texture.h"
#include "ogldev_engine_common.h"
#include <lights_common.h>

class Terrain : public Technique
{
  public:
    bool Init(const std::string& file);
    void Render();
    void SetMVP(glm::mat4 proj, glm::mat4 view, glm::mat4 model, glm::mat4 normal);
    void SetRender(float x, float y, float z);
    void SetLight(glm::vec3 color, glm::vec3 dir, float amb);

  private:
    Magick::Image m_image;
    Magick::Blob m_blob;
    GLuint VB;
    GLuint IB;
    int iRows, iCols;
    glm::vec3 vRenderScale;
    GLint RenderHeight;
    GLint MaxTextureU;
    GLint MaxTextureV;
    GLint HeightmapScale;
    GLint ProjMatrix;
    GLint ViewMatrix;
    GLint ModelMatrix;
    GLint NormalMatrix;
    GLint Color;

    struct DirectionalLight
    {
      GLint vColor;
      GLint vDirection;
      GLint vfAmbient;
    } SunLight;

};

#endif
