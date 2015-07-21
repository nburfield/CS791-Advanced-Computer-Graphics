//
// Shader Class
//
#ifndef SHADER_H
#define SHADER_H

#include <OpenGL/GLU.h>

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
