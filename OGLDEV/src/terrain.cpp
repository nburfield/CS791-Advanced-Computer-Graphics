#include <terrain.h>

bool Terrain::Init(const std::string& file)
{
  // Open the file
  try
  {
    m_image.read(file);
    m_image.write(&m_blob, "RGBA");
  }
  catch (Magick::Error& Error)
  {
    std::cout << "Error loading texture in terrain '" << file << "': " << Error.what() << std::endl;
    return false;
  }

  iRows = m_image.rows();
  iCols = m_image.columns();
  
  if(iRows == 0 || iCols == 0 || (m_image.depth() != 24 && m_image.depth() != 8))
  {
    std::cout<< "Incorrect data in the file." << std::endl;
    return false;
  }

  unsigned int ptr_inc = m_image.depth() == 24 ? 3 : 1;
  unsigned int row_step = ptr_inc * iCols;

  // Build Vertex and textures
  vector< vector< glm::vec3> > vVertexData(iRows, vector<glm::vec3>(iCols)); 
  vector< vector< glm::vec2> > vCoordsData(iRows, vector<glm::vec2>(iCols)); 

  float fTextureU = float(iCols)*0.1f; 
  float fTextureV = float(iRows)*0.1f;

  const void* data = m_blob.data();

  for(int i = 0; i < iRows; i++) 
  { 
    for(int j = 0; j < iCols; j++) 
    { 
      float fScaleC = float(j)/float(iCols-1); 
      float fScaleR = float(i)/float(iRows-1); 
      float fVertexHeight = float(*(static_cast<int const *>(data) + row_step * i + j * ptr_inc))/255.0f; 
      vVertexData[i][j] = glm::vec3(-0.5f+fScaleC, fVertexHeight, -0.5f+fScaleR); 
      vCoordsData[i][j] = glm::vec2(fTextureU*fScaleC, fTextureV*fScaleR); 
    } 
  }

  // Build normals
  vector< vector<glm::vec3> > vNormals[2]; 
  for(int i = 0; i < 2; i++)
  {
    vNormals[i] = vector< vector<glm::vec3> >(iRows-1, vector<glm::vec3>(iCols-1)); 
  }

  for(int i = 0; i < iRows-1; i++) 
  { 
    for(int j = 0; j < iCols-1; j++) 
    { 
      glm::vec3 vTriangle0[] = { vVertexData[i][j], vVertexData[i+1][j], vVertexData[i+1][j+1]}; 
      glm::vec3 vTriangle1[] = { vVertexData[i+1][j+1], vVertexData[i][j+1], vVertexData[i][j]}; 

      glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0]-vTriangle0[1], vTriangle0[1]-vTriangle0[2]); 
      glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0]-vTriangle1[1], vTriangle1[1]-vTriangle1[2]); 

      vNormals[0][i][j] = glm::normalize(vTriangleNorm0); 
      vNormals[1][i][j] = glm::normalize(vTriangleNorm1); 
    } 
  }

  // Sum Normals, and normalize
  vector< vector<glm::vec3> > vFinalNormals = vector< vector<glm::vec3> >(iRows, vector<glm::vec3>(iCols)); 

  for(int i = 0; i < iRows; i++) 
  { 
    for(int j = 0; j < iCols; j++) 
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
      if(i != 0 && j != iCols-1)
      {
        vFinalNormal += vNormals[0][i-1][j]; 
      }

      // Look for bottom-right triangles
      if(i != iRows-1 && j != iCols-1)
      {
        for(int k = 0; k < 2; k++)
        {
          vFinalNormal += vNormals[k][i][j]; 
        }
      }

      // Look for bottom-left triangles
      if(i != iRows-1 && j != 0)
      {
        vFinalNormal += vNormals[1][i][j-1]; 
      }

      vFinalNormal = glm::normalize(vFinalNormal); 
      vFinalNormals[i][j] = vFinalNormal;
    } 
  }

  // Build the buffer
  std::vector<GLM_Vertex> Vertices;
  std::vector<unsigned int> Indices;

  for(int i = 0; i < iRows; i++) 
  { 
    for(int j = 0; j < iCols; j++) 
    { 
      Vertices.push_back(GLM_Vertex(vVertexData[i][j], 
                          vCoordsData[i][j], 
                          vFinalNormals[i][j], 
                          glm::vec3(0, 0, 0)));
      printf("Vertex: %.2f, %.2f, %.2f\n", vVertexData[i][j].x, vVertexData[i][j].y, vVertexData[i][j].z);
    } 
  }

  int iPrimitiveRestartIndex = iRows * iCols; 
  for(int i = 0; i < iRows-1; i++) 
  { 
    for(int j = 0; j < iCols; j++) 
    { 
      printf("Indicies: ");
      for(int k = 0; k < 2; k++)
      { 
        int iRow = i+(1-k); 
        int iIndex = iRow*iCols+j; 
        Indices.push_back((unsigned int)iIndex);
        printf("%d, ", iIndex);
      }

      // Restart triangle strips
      printf("%d\n", iPrimitiveRestartIndex);
      Indices.push_back((unsigned int)iPrimitiveRestartIndex);
    } 
  } 

  if (!Technique::Init())
  {
    return false;
  }

  if (!AddShader(GL_VERTEX_SHADER, "../shaders/terrain_vertex.glsl"))
  {
    return false;
  }

  if (!AddShader(GL_FRAGMENT_SHADER, "../shaders/terrain_fragment.glsl"))
  {
    return false;
  }

  if (!Finalize())
  {
    return false;
  }

  RenderHeight = GetUniformLocation("fRenderHeight");
  if(RenderHeight == INVALID_UNIFORM_LOCATION)
  {
    printf("RenderHeight not found\n");
    return false;
  }

  MaxTextureU = GetUniformLocation("fMaxTextureU");
  if(MaxTextureU == INVALID_UNIFORM_LOCATION)
  {
    printf("MaxTextureU not found\n");
    return false;
  }

  MaxTextureV = GetUniformLocation("fMaxTextureV");
  if(MaxTextureV == INVALID_UNIFORM_LOCATION)
  {
    printf("MaxTextureV not found\n");
    return false;
  }

  HeightmapScale = GetUniformLocation("HeightmapScaleMatrix");
  if(HeightmapScale == INVALID_UNIFORM_LOCATION)
  {
    printf("HeightmapScale not found\n");
    return false;
  }

  ProjMatrix = GetUniformLocation("matrices.projMatrix");
  if(ProjMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("ProjMatrix not found\n");
    return false;
  }
  ViewMatrix = GetUniformLocation("matrices.viewMatrix");
  if(ViewMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("ViewMatrix not found\n");
    return false;
  }
  ModelMatrix = GetUniformLocation("matrices.modelMatrix");
  if(ModelMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("ModelMatrix not found\n");
    return false;
  }
  NormalMatrix = GetUniformLocation("matrices.normalMatrix");
  if(NormalMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("NormalMatrix not found\n");
    return false;
  }
  Color = GetUniformLocation("vColor");
  if(Color == INVALID_UNIFORM_LOCATION)
  {
    printf("Color not found\n");
    return false;
  }

  SunLight.vColor = GetUniformLocation("sunLight.vfColor");
  if(SunLight.vColor == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.vColor not found\n");
    return false;
  }

  SunLight.vDirection = GetUniformLocation("sunLight.vDirection");
  if(SunLight.vDirection == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.vDirection not found\n");
    return false;
  }

  SunLight.vfAmbient = GetUniformLocation("sunLight.fAmbient");
  if(SunLight.vfAmbient == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.fAmbient not found\n");
    return false;
  }

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLM_Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


  // Return success
  return true;
}

void Terrain::Render()
{
  glUniform1f(RenderHeight, vRenderScale.y);
  glUniform1f(MaxTextureU, float(iCols)*0.1f);
  glUniform1f(MaxTextureV, float(iRows)*0.1f);
  glUniformMatrix4fv(HeightmapScale, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0), glm::vec3(vRenderScale))));
  glUniform4fv(Color, 1, glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);                 // position
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); // texture coordinate
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); // normal
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32); // tangent

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  //texture->Bind(COLOR_TEXTURE_UNIT);

  glEnable(GL_PRIMITIVE_RESTART); 
  glPrimitiveRestartIndex(iRows*iCols);
  glDrawElements(GL_TRIANGLES, ((iRows-1)*iCols*2 + iRows-1), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
}

void Terrain::SetMVP(glm::mat4 proj, glm::mat4 view, glm::mat4 model, glm::mat4 normal)
{
  glUniformMatrix4fv(ProjMatrix, 1, GL_FALSE, glm::value_ptr(proj));
  glUniformMatrix4fv(ViewMatrix, 1, GL_FALSE, glm::value_ptr(view));  
  glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(model));  
  glUniformMatrix4fv(NormalMatrix, 1, GL_FALSE, glm::value_ptr(normal));  
}

void Terrain::SetRender(float x, float y, float z)
{
  vRenderScale = glm::vec3(x, y, z);
}

void Terrain::SetLight(glm::vec3 color, glm::vec3 dir, float amb)
{
  glUniform3fv(SunLight.vColor, 1, glm::value_ptr(color));
  glUniform3fv(SunLight.vDirection, 1, glm::value_ptr(dir));
  glUniform1f(SunLight.vfAmbient, amb);
}
