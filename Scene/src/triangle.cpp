
#include <limits.h>
#include <string.h>

#include <triangle.h>

Triangle::Triangle()
{   
}

bool Triangle::Initilize()
{
  if (!Technique::Init()) {
      return false;
  }

  if (!AddShader(GL_VERTEX_SHADER, "../shaders/triangle_vertex.glsl"))
  {
    return false;
  }

  if (!AddShader(GL_FRAGMENT_SHADER, "../shaders/triangle_fragment.glsl"))
  {
    return false;
  }

  if (!Finalize()) {
      return false;
  }

  m_WVPLocation = GetUniformLocation("mvpMatrix");
  if (m_WVPLocation == INVALID_UNIFORM_LOCATION)
  {
    return false;
  }


  std::vector<GLM_Vertex> Vertices;
  std::vector<unsigned int> Indices;


  Vertices.push_back(GLM_Vertex(glm::vec3(-1.0f, -1.0f, 0.5773f), 
                                glm::vec2(0.0f, 0.0f), 
                                glm::vec3(0, 0, 0), 
                                glm::vec3(0, 0, 0)));

  Vertices.push_back(GLM_Vertex(glm::vec3(0.0f, -1.0f, -1.15475f), 
                                glm::vec2(0.5f, 0.0f), 
                                glm::vec3(0, 0, 0), 
                                glm::vec3(0, 0, 0)));

  Vertices.push_back(GLM_Vertex(glm::vec3(1.0f, -1.0f, 0.5773f), 
                                glm::vec2(1.0f, 0.0f), 
                                glm::vec3(0, 0, 0), 
                                glm::vec3(0, 0, 0)));

  Vertices.push_back(GLM_Vertex(glm::vec3(0.0f, 1.0f, 0.0f), 
                                glm::vec2(0.5f, 1.0f), 
                                glm::vec3(0, 0, 0), 
                                glm::vec3(0, 0, 0)));

  Vertices.push_back(GLM_Vertex(glm::vec3(5, 0, -5), 
                                glm::vec2(1.0, 1.0), 
                                glm::vec3(0, 0, 0), 
                                glm::vec3(0, 0, 0)));


  Indices.push_back(0);
  Indices.push_back(3);
  Indices.push_back(1);
  Indices.push_back(1);
  Indices.push_back(3);
  Indices.push_back(2);
  Indices.push_back(2);
  Indices.push_back(3);
  Indices.push_back(0);
  Indices.push_back(0);
  Indices.push_back(1);
  Indices.push_back(2);
  

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLM_Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  texture = new Texture(GL_TEXTURE_2D, "../content/pirate.jpg");

  if (!texture->Load())
  {
      return false;
  }

  return true;
}

void Triangle::Render(glm::vec3 loc, glm::mat4 view, glm::mat4 proj)
{
    Enable();
    glm::mat4 mvp = proj * view ;//* glm::translate(glm::mat4(1.0f), loc);

    glDepthMask(GL_TRUE);    
    glEnable(GL_DEPTH_TEST);    
    glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLM_Vertex), 0);                 // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLM_Vertex), (const GLvoid*)12); // texture coordinate
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLM_Vertex), (const GLvoid*)20); // normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLM_Vertex), (const GLvoid*)32); // tangent

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    texture->Bind(GL_TEXTURE0);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    glDepthMask(GL_FALSE);
}

