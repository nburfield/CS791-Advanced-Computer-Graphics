
#include <limits.h>
#include <string.h>

#include <flag_technique.h>

FlagTechnique::FlagTechnique()
{   
}

bool FlagTechnique::Initilize()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "../shaders/flag_vertex.glsl")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "../shaders/flag_fragment.glsl")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    
    m_WVPLocation = GetUniformLocation("mvpMatrix");

  if (m_WVPLocation == INVALID_UNIFORM_LOCATION) {
    return false;
  }

  m_modelMatrix = GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) {
    return false;
  }

    loc_waveTime = GetUniformLocation("waveTime");
  if(loc_waveTime == INVALID_UNIFORM_LOCATION)
  {
    printf("loc_waveTime not found\n");
    return false;
  }

  loc_waveWidth = GetUniformLocation("waveWidth");
  if(loc_waveWidth == INVALID_UNIFORM_LOCATION)
  {
    printf("loc_waveWidth not found\n");
    return false;
  }

  loc_waveHeight = GetUniformLocation("waveHeight");
  if(loc_waveHeight == INVALID_UNIFORM_LOCATION)
  {
    printf("loc_waveHeight not found\n");
    return false;
  }

    waveTime = 0.5; waveWidth = 5.3; waveHeight = 1.0; waveFreq = 0.05;

    std::vector<GLM_Vertex> Vertices;
    std::vector<unsigned int> Indices;

    width = 10; height = 10;
    increment = 1.0;

    
    for(float i = 0.0; i <= width; i+= increment)
    {
        for(float j = 0.0; j <= height; j+=increment)
        {
          Vertices.push_back(GLM_Vertex(glm::vec3(i, j, 0), 
                                    glm::vec2((i/(float)width), (j/(float)height)), 
                                    glm::vec3(0, 0, 0), 
                                    glm::vec3(0, 0, 0)));
        }
    }

  // Build normals
    /*
  vector< vector<glm::vec3> > vNormals[2]; 
  for(int i = 0; i < 2; i++)
  {
    vNormals[i] = vector< vector<glm::vec3> >(width-1, vector<glm::vec3>(height-1)); 
  }

  for(int i = 0; i < height-1; i++) 
  { 
    for(int j = 0; j < width-1; j++) 
    { 
      glm::vec3 vTriangle0[] = { Vertices[(i * height)+j+1].m_pos, Vertices[(i * height)+j].m_pos, Vertices[((1+i) * height)+j+1].m_pos}; 
      glm::vec3 vTriangle1[] = { Vertices[((1+i) * height)+j+2].m_pos, Vertices[(i * height)+j+1].m_pos, Vertices[((1+i) * height)+j+1].m_pos}; 

      glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0]-vTriangle0[1], vTriangle0[1]-vTriangle0[2]); 
      glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0]-vTriangle1[1], vTriangle1[1]-vTriangle1[2]); 

      vNormals[0][i][j] = glm::normalize(vTriangleNorm0); 
      vNormals[1][i][j] = glm::normalize(vTriangleNorm1); 
    } 
  }

  // Sum Normals, and normalize
  for(int i = 0; i < height; i++) 
  { 
    for(int j = 0; j < width; j++) 
    { 
      glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f); 

      // Look for upper-left triangles
      if(j != 0 && i != 0) 
      {
        for(int k = 0; k < 2; k++)
        {
          vFinalNormal += vNormals[k][i-1][j-1]; 
        }
      }

      // Look for upper-right triangles
      if(i != 0 && j != height-1)
      {
        vFinalNormal += vNormals[0][i-1][j]; 
      }

      // Look for bottom-right triangles
      if(i != height-1 && j != height-1)
      {
        for(int k = 0; k < 2; k++)
        {
          vFinalNormal += vNormals[k][i][j]; 
        }
      }

      // Look for bottom-left triangles
      if(i != height-1 && j != 0)
      {
        vFinalNormal += vNormals[1][i][j-1]; 
      }

      vFinalNormal = glm::normalize(vFinalNormal);
      Vertices[(i*height) + j].m_normal = vFinalNormal;
    } 
  }
  */
  
    int scale_width = width/increment;
    int scale_height = height/increment;
    for(int i = 0; i < scale_height; i++)
    {
      for(int j = 0; j < scale_width; j++)
      {
        
        Indices.push_back((i * scale_height)+j+1);
        Indices.push_back((i * scale_height)+j);
        Indices.push_back(((1+i) * scale_height)+j+1);

        Indices.push_back(((1+i) * scale_height)+j+2);
        Indices.push_back((i * scale_height)+j+1);
        Indices.push_back(((1+i) * scale_height)+j+1);
      }
    }
    

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

void FlagTechnique::Render(glm::vec3 loc, glm::mat4 view, glm::mat4 proj)
{
    Enable();
    waveTime += waveFreq;
    glm::mat4 model = glm::translate(glm::mat4(1.0f), loc) * glm::rotate(glm::mat4(1.0f), -1.5708f, glm::vec3(0,1,0));
    glm::mat4 mvp = proj * view * model;

    glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(model)); 
    glUniform1f(loc_waveTime, waveTime);
    glUniform1f(loc_waveWidth, waveWidth);
    glUniform1f(loc_waveHeight, waveHeight);

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

    int value = ((width/increment) * (height/increment)) * 6;
    glDrawElements(GL_TRIANGLES, value, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

}

