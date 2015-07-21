//
// Shader Class
//
#ifndef SHADER_H
#define SHADER_H

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/GLU.h>
#else //linux as default
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
