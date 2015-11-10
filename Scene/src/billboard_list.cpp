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

/*
  glm::vec3 Positions[NUM_ROWS * NUM_COLUMNS];
  float counter_x = -10.0;
  float counter_y = -10.0;
  for (unsigned int j = 0 ; j < NUM_ROWS ; j++)
  {
    for (unsigned int i = 0 ; i < NUM_COLUMNS ; i++)
    {
      glm::vec3 Pos(counter_x, 0.0f, counter_y);
      Positions[j * NUM_COLUMNS + i] = Pos;
      counter_x = counter_x + 0.1;
    }
    counter_x = 0.0f;
    counter_y = counter_y + 0.1;
  }

  glGenBuffers(1, &m_VB);
  glBindBuffer(GL_ARRAY_BUFFER, m_VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Positions), &Positions[0], GL_STATIC_DRAW);


  for(unsigned int i = 0; i < vertices.size(); i++)
  {
    if(vertices[i].m_pos.y >= 0.3f && vertices[i].m_pos.y < 0.65f)
    {
      Positions.push_back(glm::vec3(vertices[i].m_pos.x * RenderScale.x, vertices[i].m_pos.y * RenderScale.y, vertices[i].m_pos.z * RenderScale.z));
    }
  }
*/


void BillboardList::Render(int m_delta, const glm::vec3 CameraPos, const glm::mat4 VP)
{
    m_time += m_delta;
    m_technique.Enable();
    m_technique.SetVP(VP);
    m_technique.SetDelta(m_delta);
    m_technique.SetTime(m_time);
    m_technique.SetCameraPosition(CameraPos);
    m_technique.SetBillboardSize(5.14f);
    
    m_pTexture->Bind(GL_TEXTURE0);
    
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);   // position
    
    glDrawArrays(GL_POINTS, 0, size);
    
    glDisableVertexAttribArray(0);
}
