#ifndef TERRAIN_H
#define TERRAIN_H

#include <technique.h>
#include <map>
#include <vector>
#include <texture.h>
#include <lights_common.h>
#include <utility.h>

// GDAL includes
//gdal
//#include <gdal.h>
//#include <gdal_priv.h>
//#include <gdalwarper.h>
//#include <ogrsf_frmts.h>

class Terrain : public Technique
{
  public:
    Terrain(glm::vec3 loc, glm::vec3 scale);
    ~Terrain();
    bool Initilize(const std::string& file);
    void Render(glm::mat4 view, glm::mat4 proj, float dt);
    bool buildTerrain(const std::string& file);
    bool ToggleNight();
    //bool gdalTerrain(const std::string& file);
    //glm::vec3 ComputeNormal(glm::vec3 center, int i, int j, int width, int height, vector<vector<float>>& data, float Max, float xres, float yres );
    //void ComputeGeoProperties(GDALDataset *poDataset, int width, int height, double& x, double& y, double& xright, double& ybottom, double& xres, double& yres);


    std::vector<GLM_Vertex> Vertices;
    std::vector<glm::vec3> GrassVertices;
    glm::vec3 vRenderScale;
  private:
    void SetLight();

    Magick::Image m_image;
    Magick::Blob m_blob;
    GLuint VB;
    GLuint IB;
    int iRows, iCols, size;
    glm::mat4 model;
    glm::vec3 location;
    GLint RenderHeight;
    GLint MaxTextureU;
    GLint MaxTextureV;
    GLint HeightmapScale;
    GLint ProjMatrix;
    GLint ViewMatrix;
    GLint ModelMatrix;
    GLint NormalMatrix;
    GLint Color;
    std::vector<Texture*> texture;
    GLint TextureLocations[5];
    PointLight Sun;
    float spin;
    bool night;

    struct
    {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint DiffuseIntensity;
      GLuint Position;
      struct
      {
        GLuint Constant;
        GLuint Linear;
        GLuint Exp;
      } Atten;
    } SunLight;

};

#endif
