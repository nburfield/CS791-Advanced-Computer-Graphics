

#ifndef PROGRAMHANDLE_H
#define PROGRAMHANDLE_H

#include <OpenGL/GLU.h>
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <OpenGL/gl3.h>

#include <shader.h>

class Program
{
  public:
    bool Init();
    bool Start();

  protected:
    GLuint program;
};

#endif
