#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <opengl_data.h>
#include <technique.h>
#include <map>
#include <vector>
#include <texture.h>
#include <utility.h>
#include <lights_common.h>

class Triangle : public Technique {
public:

    Triangle();

    virtual bool Initilize();

    void Render(glm::vec3 loc, glm::mat4 view, glm::mat4 proj);

private:
    GLuint m_WVPLocation; 
    GLuint VB;
    GLuint IB;
    Texture *texture;
};


#endif
