

#ifndef PROGRAMHANDLE_H
#define PROGRAMHANDLE_H

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  #include <GL/glu.h>
#endif

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
