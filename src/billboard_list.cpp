#include <billboard_list.h>

#define NUM_ROWS 100
#define NUM_COLUMNS 100


BillboardList::BillboardList()
{
    m_pTexture = NULL;
    m_VB = INVALID_OGL_VALUE;
    m_time = 0;
}


BillboardList::~BillboardList()
{
    SAFE_DELETE(m_pTexture);
    
    if (m_VB != INVALID_OGL_VALUE)
    {
        glDeleteBuffers(1, &m_VB);
    }
}
    
    
bool BillboardList::Initilize(const std::string& TexFilename, std::vector<glm::vec3> vertices, glm::vec3 RenderScale)
{
    m_pTexture = new Texture(GL_TEXTURE_2D, TexFilename.c_str());
        
    if (!m_pTexture->Load()) {
        return false;
    }

    CreatePositionBuffer(vertices, RenderScale);
    
    if (!m_technique.Initilize("billboard_grass")) {
        return false;
    }

    /*
    m_view = m_technique.GetUniformLocation("view");
    if (m_view == INVALID_UNIFORM_LOCATION) {
        return false;        
    }
    */
    
    return true;
}


void BillboardList::CreatePositionBuffer(std::vector<glm::vec3> vertices, glm::vec3 RenderScale)
{    
  std::vector<glm::vec3> Positions;

  for(unsigned int i = 0; i < vertices.size(); i++)
  {
    Positions.push_back(glm::vec3(vertices[i].x * RenderScale.x, vertices[i].y * RenderScale.y, vertices[i].z * RenderScale.z));
  }

  glGenBuffers(1, &m_VB);
  glBindBuffer(GL_ARRAY_BUFFER, m_VB);
  size = Positions.size();
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
}

void BillboardList::Render(int m_delta, const glm::vec3 CameraPos, const glm::mat4 VP, const glm::mat4 view)
{
    m_time += m_delta;
    m_technique.Enable();
    m_technique.SetVP(VP);
    m_technique.SetDelta(m_delta);
    m_technique.SetTime(m_time);
    m_technique.SetCameraPosition(CameraPos);
    m_technique.SetBillboardSize(5.14f);
    glUniformMatrix4fv(m_view, 1, GL_FALSE, glm::value_ptr(view));
    
    m_pTexture->Bind(GL_TEXTURE0);
    
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);   // position
    
    glDrawArrays(GL_POINTS, 0, size);
    
    glDisableVertexAttribArray(0);
}
