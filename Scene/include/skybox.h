

#ifndef SKYBOX_H
#define SKYBOX_H

#include <skybox_tech.h>
#include <cube_map_texture.h>
#include <mesh.h>

#include <opengl_data.h>

class SkyBox
{
public:
    SkyBox();

    ~SkyBox();
    
    bool Init(const string& Directory,
              const string& PosXFilename,
              const string& NegXFilename,
              const string& PosYFilename,
              const string& NegYFilename,
              const string& PosZFilename,
              const string& NegZFilename);
    
    void Render(glm::mat4 projection, glm::mat4 view, glm::mat4 camLoc);
    
private:    
    SkyboxTechnique* m_pSkyboxTechnique;
    CubemapTexture* m_pCubemapTex;
    Mesh* m_pMesh;
};

#endif  /* SKYBOX_H */

