//
// Shader Class
//

#include <GL/glew.h> // glew must be included before the main gl libs
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

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
