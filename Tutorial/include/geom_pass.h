/*
  Used as the geometry pass for the Deferred Shading
*/

#ifndef GEOM_PASS_H
#define GEOM_PASS_H

#include <opengl_data.h>
#include <programHandle.h>

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
