#ifndef DS_DIR_LIGHT_PASS_TECH_H
#define DS_DIR_LIGHT_PASS_TECH_H

#include <opengl_data.h>
#include <ds_light_pass_tech.h>
#include <lights_common.h>
#include <utility.h>
#include <limits.h>
#include <string.h>

class DSDirLightPassTech : public DSLightPassTech
{
  public:

    DSDirLightPassTech();
    virtual bool Init();
    void SetDirectionalLight(const DirectionalLight& Light);

  private:

    struct
    {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint DiffuseIntensity;
      GLuint Direction;
    } m_dirLightLocation;
};


#endif
