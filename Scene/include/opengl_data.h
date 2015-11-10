#ifndef OPENGL_DATA_H
#define OPENGL_DATA_H

#include <iostream>
using namespace std;

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  #include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

struct GLM_Vertex
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;
    glm::vec3 m_tangent;

    GLM_Vertex() {}

    GLM_Vertex(const glm::vec3 pos, const glm::vec2 tex, const glm::vec3 normal, const glm::vec3 Tangent)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
        m_tangent = Tangent;
                
    }
};


#endif
