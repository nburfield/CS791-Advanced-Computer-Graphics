#ifndef RANDOM_TEXTURE_H
#define RANDOM_TEXTURE_H

#include <opengl_data.h>
#include <utility.h>

class RandomTexture
{
public:
    RandomTexture();

    ~RandomTexture();
    
    bool InitRandomTexture(unsigned int Size);

    void Bind(GLenum TextureUnit);

private:
    GLuint m_textureObj;
};



#endif  /* RANDOM_TEXTURE_H */

