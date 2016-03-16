#ifndef PS_UPDATE_TECHNIQUE_H
#define PS_UPDATE_TECHNIQUE_H

#include <technique.h>
#include <utility.h>

class PSUpdateTechnique : public Technique
{
public:
    PSUpdateTechnique();
    
    virtual bool Initilize();    
    
    void SetParticleLifetime(float Lifetime);
    
    void SetDeltaTimeMillis(unsigned int DeltaTimeMillis);
    
    void SetTime(int Time);

    void SetRandomTextureUnit(unsigned int TextureUnit);
    
    void SetLauncherLifetime(float Lifetime);
    
    void SetShellLifetime(float Lifetime);
    
    void SetSecondaryShellLifetime(float Lifetime);

    void SetRandomColor();
    
 private:
    GLuint m_deltaTimeMillisLocation;
    GLuint m_randomTextureLocation;
    GLuint m_timeLocation;
    GLuint m_launcherLifetimeLocation;
    GLuint m_shellLifetimeLocation;
    GLuint m_secondaryShellLifetimeLocation;
    GLuint m_newShellColor;
};


#endif  /* PS_UPDATE_TECHNIQUE_H */

