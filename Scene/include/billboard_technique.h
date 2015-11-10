#ifndef BILLBOARD_TECHNIQUE_H
#define BILLBOARD_TECHNIQUE_H

#include <technique.h>
#include <opengl_data.h>
#include <utility.h>

class BillboardTechnique : public Technique 
{
public:
    
    BillboardTechnique();
 
    virtual bool Initilize(string shader);
    
    void SetVP(const glm::mat4 VP);
    void SetCameraPosition(const glm::vec3 Pos);
    void SetColorTextureUnit(unsigned int TextureUnit);
    void SetBillboardSize(float BillboardSize);
    void SetDelta(unsigned int dt);
    void SetTime(int Time);
    
private:

    GLuint m_VPLocation;
    GLuint m_cameraPosLocation;
    GLuint m_colorMapLocation;
    GLuint m_billboardSizeLocation;
    GLuint m_DeltaTime;
    GLuint m_Time;
};

#endif  /* BILLBOARD_TECHNIQUE_H */

