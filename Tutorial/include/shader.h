//
// Shader Class
//
#ifndef SHADER_H
#define SHADER_H

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  #include <GL/glu.h>
#endif

class Shader
   {
    public:
        //Needed Functions
        Shader();
        Shader(Shader &src);
        ~Shader();
      
        //Working functions
        bool Initilize(const char * fileLoc, GLenum shaderType, GLuint &program);

    private:
        bool readFile(const char *fileLoc);
        char* buffer;
        long bufferSize;
        GLuint shader;
        GLenum _shaderType;
   };
#endif
