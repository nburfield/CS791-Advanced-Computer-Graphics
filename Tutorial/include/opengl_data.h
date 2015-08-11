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

#ifdef WIN32
#include <Windows.h>
#define _USE_MATH_DEFINES 
#include <cmath>
#else
#include <sys/time.h>
#include <math.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff
#define RANDOM_TEXTURE_UNIT GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX 3
#define COLOR_TEXTURE_UNIT GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX 0

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))
#define RANDOM random


struct BasicVertex
{
    float m_pos[3];
    float m_tex[2];
    float m_normal[3];

    BasicVertex() {}

    BasicVertex(const glm::vec3 pos, const glm::vec2 tex, const glm::vec3 normal)
    {
        m_pos[0]    = pos.x;
        m_pos[1]    = pos.y;
        m_pos[2]    = pos.z;
        m_tex[0]    = tex.x;
        m_tex[1]    = tex.y;
        m_normal[0] = normal.x;
        m_normal[1] = normal.y;
        m_normal[2] = normal.z;
    }
};

struct Vertex
{
    float m_pos[3];
    float m_tex[2];
    float m_normal[3];
    float m_tangent[3];

    Vertex() {}

    Vertex(const glm::vec3 pos, const glm::vec2 tex, const glm::vec3 normal, const glm::vec3 Tangent)
    {
        m_pos[0]    = pos.x;
        m_pos[1]    = pos.y;
        m_pos[2]    = pos.z;
        m_tex[0]    = tex.x;
        m_tex[1]    = tex.y;
        m_normal[0] = normal.x;
        m_normal[1] = normal.y;
        m_normal[2] = normal.z;
        m_tangent[0] = Tangent.x;
        m_tangent[1] = Tangent.y;
        m_tangent[2] = Tangent.z;
                
    }
};

std::string ErrorString(GLenum error);
long long GetCurrentTimeMillis();
float RandomFloat();

#endif
