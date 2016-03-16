#ifndef MESH_H
#define MESH_H

#include <map>
#include <vector>
#include <string>
using namespace std;

#include <opengl_data.h>
    
//Assimp
#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h>
#include <utility.h>
#include <texture.h>

class Mesh
{
public:
    Mesh();

    ~Mesh();

    bool LoadMesh(const string& Filename);

    void Render();

private:
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
        MeshEntry();

        ~MeshEntry();

        bool Init(const std::vector<GLM_Vertex>& Vertices,
                  const std::vector<unsigned int>& Indices);

        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
};


#endif  /* MESH_H */

