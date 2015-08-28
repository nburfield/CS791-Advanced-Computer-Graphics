
#ifndef WATER_TECHNIQUE_H
#define WATER_TECHNIQUE_H

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  #include <GL/glu.h>
#endif

#include "technique.h"
#include "ogldev_math_3d.h"
#include <map>
#include <vector>
#include "ogldev_texture.h"
#include "ogldev_engine_common.h"


class WaterTechnique : public Technique {
public:

    WaterTechnique();

    virtual bool Init();

    void Render(const Matrix4f& VP, const Vector3f& CameraPos);
    void SetWVP(const Matrix4f& WVP);
    void SetModel(const Matrix4f& WVP);

private:
    Matrix4f model;
    GLuint m_WVPLocation; 
    GLuint VB;
    GLuint IB;
    Texture *texture;
    int size;
};


#endif
