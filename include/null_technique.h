

#ifndef NULL_TECHNIQUE_H
#define NULL_TECHNIQUE_H

#include <limits.h>
#include <string.h>
#include <utility.h>

#include <technique.h>
#include <opengl_data.h>

class NullTechnique : public Technique {
public:

    NullTechnique();

    virtual bool Init();

    void SetWVP(const glm::mat4 WVP);

private:

    GLuint m_WVPLocation;
};


#endif
