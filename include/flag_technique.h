#ifndef FLAG_TECHNIQUE_H
#define FLAG_TECHNIQUE_H

#include <opengl_data.h>
#include <technique.h>
#include <map>
#include <vector>
#include <texture.h>
#include <utility.h>

class FlagTechnique : public Technique {
public:

    FlagTechnique();

    virtual bool Initilize();

    void Render(glm::vec3 loc, glm::mat4 view, glm::mat4 proj);

private:
    GLuint m_WVPLocation;
    GLuint m_modelMatrix;
    GLuint VB;
    GLuint IB;
    Texture *texture;
    int width, height;
    float increment;
    float waveTime, waveWidth, waveHeight, waveFreq;
    GLint loc_waveTime;
    GLint loc_waveWidth;
    GLint loc_waveHeight;
};


#endif
