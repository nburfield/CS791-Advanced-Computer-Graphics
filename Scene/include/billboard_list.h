#ifndef BILLBOARD_LIST_H
#define BILLBOARD_LIST_H

#include <string>

#include <texture.h>
#include <billboard_technique.h>
#include <utility.h>
#include <opengl_data.h>
#include <vector>

class BillboardList
{
public:
    BillboardList();    
    ~BillboardList();
    
    bool Initilize(const std::string& TexFilename, std::vector<glm::vec3> vertices, glm::vec3 RenderScale);
    
    void Render(int DeltaTimeMillis, const glm::vec3 CameraPos, const glm::mat4 VP);

private:
    void CreatePositionBuffer(std::vector<glm::vec3> vertices, glm::vec3 RenderScale);
    
    GLuint m_VB;
    Texture* m_pTexture;
    BillboardTechnique m_technique;
    int m_time;
    int size;
};


#endif  /* BILLBOARD_LIST_H */
