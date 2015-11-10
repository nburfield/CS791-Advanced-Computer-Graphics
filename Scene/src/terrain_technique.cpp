#include <terrain_technique.h>

Terrain::Terrain(glm::vec3 loc, glm::vec3 scale)
{
  location = loc;
  model = glm::translate(glm::mat4(1.0f), loc);
  vRenderScale = scale;
  night = false;

  Sun.AmbientIntensity = 0.2f;
  Sun.DiffuseIntensity = 0.2f;
  Sun.Color = glm::vec3(1.0f, 1.0f, 1.0f);
  Sun.Position = glm::vec3(-18.60f, 242.30f, -298.37f);
  Sun.Attenuation.Constant = 0.0f;
  Sun.Attenuation.Linear = 0.0f;
  Sun.Attenuation.Exp = 0.03f;

  spin = 7.216546f;
}


Terrain::~Terrain()
{

}


bool Terrain::Initilize(const std::string& file)
{
  if (!Technique::Init())
  {
    return false;
  }

  if (!AddShader(GL_VERTEX_SHADER, "../shaders/terrain_vertex.glsl"))
  {
    return false;
  }
/*
  if (!AddShader(GL_GEOMETRY_SHADER, "../shaders/terrain_geometry.glsl"))
  {
    return false;
  }
*/
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

  SunLight.Color = GetUniformLocation("sunLight.Base.Color");
  if(SunLight.Color == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.Color not found.\n");
    return false;
  }
  SunLight.AmbientIntensity = GetUniformLocation("sunLight.Base.AmbientIntensity");
  if(SunLight.AmbientIntensity == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.AmbientIntensity not found.\n");
    return false;
  }
  SunLight.DiffuseIntensity = GetUniformLocation("sunLight.Base.DiffuseIntensity");
  if(SunLight.DiffuseIntensity == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.DiffuseIntensity not found.\n");
    return false;
  }
  SunLight.Position = GetUniformLocation("sunLight.Position");
  if(SunLight.Position == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.Position not found.\n");
    return false;
  }
  SunLight.Atten.Constant = GetUniformLocation("sunLight.Atten.Constant");
  if(SunLight.Atten.Constant == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.Atten.Constant not found.\n");
    return false;
  }
  SunLight.Atten.Linear = GetUniformLocation("sunLight.Atten.Linear");
  if(SunLight.Atten.Linear == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.Atten.Linear not found.\n");
    return false;
  }
  SunLight.Atten.Exp = GetUniformLocation("sunLight.Atten.Exp");
  if(SunLight.Atten.Exp == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.Atten.Exp not found.\n");
    return false;
  }


  TextureLocations[0] = GetUniformLocation("gSampler[0]");
  if(TextureLocations[0] == INVALID_UNIFORM_LOCATION)
  {
    printf("TextureLocations[0] not found.\n");
    return false;
  } 

  TextureLocations[1] = GetUniformLocation("gSampler[1]");
  if(TextureLocations[1] == INVALID_UNIFORM_LOCATION)
  {
    printf("TextureLocations[1] not found.\n");
    return false;
  } 

  TextureLocations[2] = GetUniformLocation("gSampler[2]");
  if(TextureLocations[2] == INVALID_UNIFORM_LOCATION)
  {
    printf("TextureLocations[2] not found.\n");
    return false;
  } 

  TextureLocations[3] = GetUniformLocation("gSampler[3]");
  if(TextureLocations[3] == INVALID_UNIFORM_LOCATION)
  {
    printf("TextureLocations[3] not found.\n");
    return false;
  } 

  TextureLocations[4] = GetUniformLocation("gSampler[4]");
  if(TextureLocations[4] == INVALID_UNIFORM_LOCATION)
  {
    printf("TextureLocations[4] not found.\n");
    return false;
  } 


  texture.resize(5);
  texture[0] = new Texture(GL_TEXTURE_2D, "../Content/fungus.dds");
  if (!texture[0]->Load())
  {
    printf("Texture 0 failed\n");
    return false;
  }

  texture[1] = new Texture(GL_TEXTURE_2D, "../Content/sand_grass_02.jpg");
  if (!texture[1]->Load())
  {
    printf("Texture 1 failed\n");
    return false;
  }

  texture[2] = new Texture(GL_TEXTURE_2D, "../Content/rock_2_4w.jpg");
  if (!texture[2]->Load())
  {
    printf("Texture 2 failed\n");
    return false;
  }

  texture[3] = new Texture(GL_TEXTURE_2D, "../Content/sand.jpg");
  if (!texture[3]->Load())
  {
    printf("Texture 3 failed\n");
    return false;
  }

  texture[4] = new Texture(GL_TEXTURE_2D, "../Content/TahoeBasin_Overlay.jpg");
  if (!texture[4]->Load())
  {
    printf("Texture 4 failed\n");
    return false;
  }

  // Build the terrain
  return buildTerrain(file);
  //return gdalTerrain(file);
}

void Terrain::Render(glm::mat4 view, glm::mat4 proj, float dt)
{
  Enable();

  // Model view Projection Normal
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glUniformMatrix4fv(ProjMatrix, 1, GL_FALSE, glm::value_ptr(proj));
  glUniformMatrix4fv(ViewMatrix, 1, GL_FALSE, glm::value_ptr(view));  
  glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(model));  
  glUniformMatrix4fv(NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

  // Render Scale and Stuff
  glUniform1f(RenderHeight, vRenderScale.y);
  glUniform1f(MaxTextureU, float(iCols)*0.1f);
  glUniform1f(MaxTextureV, float(iRows)*0.1f);
  glUniformMatrix4fv(HeightmapScale, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0), glm::vec3(vRenderScale))));
  glUniform4fv(Color, 1, glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

  // Set Light
  if(night)
  {
    if(cos(spin) > 0.0 || sin(spin) > -0.7)
    {
      spin += dt * M_PI / 4;
    }
    else
    {
      Sun.AmbientIntensity = 0.18f;
      Sun.DiffuseIntensity = 0.18f;
      Sun.Color = glm::vec3(0.6f, 0.6f, 0.6f);
    }
  }
  else
  {
    if(cos(spin) < 0.5 || sin(spin) < 0.8)
    {
      spin += dt * M_PI / 4;
    }
    else
    {
      Sun.AmbientIntensity = 0.2f;
      Sun.DiffuseIntensity = 0.2f;
      Sun.Color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
  }
  
  //spin = spin * (78/5);
  SetLight();

  // Texture Data
  glUniform1i(TextureLocations[0], 0);
  glUniform1i(TextureLocations[1], 1);
  glUniform1i(TextureLocations[2], 2);
  glUniform1i(TextureLocations[3], 3);
  glUniform1i(TextureLocations[4], 4);

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

  texture[0]->Bind(GL_TEXTURE0);
  texture[1]->Bind(GL_TEXTURE1);
  texture[2]->Bind(GL_TEXTURE2);
  texture[3]->Bind(GL_TEXTURE3);
  texture[4]->Bind(GL_TEXTURE4);

  glEnable(GL_PRIMITIVE_RESTART); 
  glPrimitiveRestartIndex(iRows*iCols);
  glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisable(GL_CULL_FACE);
}

void Terrain::SetLight()
{
  Sun.Position = glm::vec3(500.0 * cos(spin), 500.0 * sin(spin), 0.0);

  glUniform3fv(SunLight.Color, 1, glm::value_ptr(Sun.Color));
  glUniform1f(SunLight.AmbientIntensity, Sun.AmbientIntensity);
  glUniform1f(SunLight.DiffuseIntensity, Sun.DiffuseIntensity);
  glUniform3fv(SunLight.Position, 1, glm::value_ptr(Sun.Position));
  glUniform1f(SunLight.Atten.Constant, Sun.Attenuation.Constant);
  glUniform1f(SunLight.Atten.Linear, Sun.Attenuation.Linear);
  glUniform1f(SunLight.Atten.Exp, Sun.Attenuation.Exp);
}

bool Terrain::ToggleNight()
{
  night = !night;
  return night;
}


bool Terrain::buildTerrain(const std::string& file)
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

  Magick::Image m_image_overlay;
  Magick::Blob m_blob_overlay;

  try
  {
    m_image_overlay.read("../content/TahoeBasin_Overlay.jpg");
  }
  catch (Magick::Error& Error)
  {
    std::cout << "Error loading texture in terrain 'Overlay': " << Error.what() << std::endl;
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

  Magick::PixelPacket *pixels = m_image_overlay.getPixels(0, 0, m_image_overlay.columns(), m_image_overlay.rows());

  for(int i = 0; i < iRows; i++) 
  { 
    for(int j = 0; j < iCols; j++) 
    { 
      float fScaleC = float(j)/float(iCols-1); 
      float fScaleR = float(i)/float(iRows-1); 
      
      float fVertexHeight = float(*(static_cast<int const *>(data) + row_step * i + j * ptr_inc))/65535.0f; 
      //printf("The Height: %.5f\n", -fVertexHeight/255.0f);
      Magick::Color color = *pixels++;
      Magick::ColorRGB rgb(color);

      //printf("Color1(rgb): %u, %u, %u\n", rgb.red(), rgb.green(), rgb.blue());
      //printf("Color2(rgb): %u, %u, %u\n", color.redQuantum(), color.greenQuantum(), color.blueQuantum());

      if(float(color.redQuantum()) < 300 && float(color.greenQuantum()) > 60000 && float(color.blueQuantum()) < 300)
      {
        GrassVertices.push_back(glm::vec3(-0.5f+fScaleC, 1-(-fVertexHeight/255.0f), -0.5f+fScaleR));
        //fVertexHeight = -70.0;
        //std::cout << fVertexHeight << std::endl;
        //std::cout << float(color.redQuantum()) << ", " << float(color.greenQuantum()) << ", " << float(color.blueQuantum()) << std::endl;
      }
      

      vVertexData[i][j] = glm::vec3(-0.5f+fScaleC, 1-(-fVertexHeight/255.0f), -0.5f+fScaleR); 
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
  std::vector<unsigned int> Indices;

  for(int i = 0; i < iRows; i++) 
  { 
    for(int j = 0; j < iCols; j++) 
    { 
      //printf("Vertex Data: %.2f, %.2f, %.2f\n", vVertexData[i][j].x , vVertexData[i][j].y , vVertexData[i][j].z);
      Vertices.push_back(GLM_Vertex(glm::vec3(vVertexData[i][j].x , vVertexData[i][j].y , vVertexData[i][j].z ), 
                          vCoordsData[i][j], 
                          vFinalNormals[i][j], 
                          glm::vec3(0, 0, 0)));
    } 
  }

  // int iPrimitiveRestartIndex = iRows * iCols; 

  printf("iRows: %i, iCols: %i\n", iRows, iCols);
  for(int i = 0; i < iRows-1; i++) 
  { 
    for(int j = 0; j < iCols-1; j++) 
    { 
     /* 
      //printf("Indicies: ");
      for(int k = 0; k < 2; k++)
      { 
        int iRow = i+(1-k); 
        int iIndex = iRow*iCols+j; 
        Indices.push_back((unsigned int)iIndex);
        //printf("%d, ", iIndex);
      }
      */
      if(j == 0 || j == 1 || j == iCols-1 || j == iCols-2)
      {
        //printf("%i, %i, %i, %i, %i, %i\n", (i * iRows)+j+1, (i * iRows)+j, ((1+i) * iRows)+j+1, ((1+i) * iRows)+j+2, (i * iRows)+j+1, ((1+i) * iRows)+j+1);
      }
        Indices.push_back((i * iRows)+j+1);
        Indices.push_back((i * iRows)+j);
        Indices.push_back(((1+i) * iRows)+j);

        Indices.push_back(((1+i) * iRows)+j);
        Indices.push_back(((1+i) * iRows)+j+1);
        Indices.push_back((i * iRows)+j+1);
       
      // Restart triangle strips
      //printf("%d\n", iPrimitiveRestartIndex);
      //Indices.push_back((unsigned int)iPrimitiveRestartIndex);

    } 
  } 

  size = Indices.size(); //((iRows-1)*iCols*2 + iRows-1);

  // Add vertex and indices's buffer
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLM_Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  return true;
}

/*
bool Terrain::gdalTerrain(const std::string& file)
{
  // 
  //vector<float> temp = vector<float>()
  GDALDataset *poDataset;


  // Register all gdal drivers
  GDALAllRegister();

  // lets load a "dataset" which is gdal terminology for your data
  poDataset = (GDALDataset*) GDALOpen(file.c_str(), GA_ReadOnly);

  // error handing!
  if (poDataset == NULL)
  {
    cout << "Failure to load file due to not existing or write permissions!!!" << endl;
    return false;
  }

  OGRSpatialReference    oUTM;

  oUTM.SetProjCS("UTM 11 / WGS84");
  oUTM.SetWellKnownGeogCS( "WGS84" );
  oUTM.SetUTM( 11 );

  //projection = string(poDataset->GetProjectionRef());

  // time to find the width of the data and print it for sanity
  cout << "Data size: " << GDALGetRasterXSize(poDataset) << " " << GDALGetRasterYSize(poDataset) << endl;

  // A new term "band" or pertentaining to one layer within a dataset
  GDALRasterBand  *poBand;

  int             nBlockXSize, nBlockYSize;
  int             bGotMin, bGotMax;
  double          adfMinMax[2];

  // Get the first band -- that's what this function does to grab one band.
  poBand = poDataset->GetRasterBand( 1 );

  // Getting the dimensions of the band... only 2D
  poBand->GetBlockSize( &nBlockXSize, &nBlockYSize );

  // Print some more handy information
  printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
          nBlockXSize, nBlockYSize,
          GDALGetDataTypeName(poBand->GetRasterDataType()),
          GDALGetColorInterpretationName(
            poBand->GetColorInterpretation()) );

  // Get the min and max
  float min = adfMinMax[0] = poBand->GetMinimum( &bGotMin );
  float max = adfMinMax[1] = poBand->GetMaximum( &bGotMax );

  // this guy will look at your current band and compute min max or you can do the above
  if ( ! (bGotMin && bGotMax) )
    GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);

  // dimensions of our datasets
  int width = poBand->GetXSize();
  int height = poBand->GetYSize();
  int W = width;
  int H = height;
  double x, y, xright, ybottom, xres, yres;
  // Time to compute the xres and yres
  ComputeGeoProperties(poDataset, width, height, x, y, xright, ybottom, xres, yres);

  double XORIGIN = x;
  double YORIGIN = y;

  // something extra not need for this simple function but you can get the number
  // of bands in the dataset
  int bands = poDataset->GetRasterCount();

  // something to hold our values!!
  float *pafScanline;

  // printing the min and max of the dataset
  std::cout << "Before allocation " << "Min: " << min << " Max: " << max << endl;

  // load the data and smile
  pafScanline = (float *) CPLMalloc(sizeof(float) * width * height);

  // I don't like dynamic float array (that is a lie) just a preference
  vector<vector<float>> input = vector<vector<float>>(width, vector<float> (height, 0));

  auto err = poBand->RasterIO(GF_Read, 0, 0, width, height, pafScanline, width, height, GDT_Float32, 0, 0);
  cout << "Loaded data with status " << err << endl;

  // Lets check the status of the loading of this dataset...
  if (err != 0)
  {
    cout << "Dataset fail to load with the following error: ";
    switch (err)
    {
    // Look up CPLErrorHandler for more information
    case 1:
      cout << "DEBUG" << endl;
      break;
    case 2:
      cout << "Warning" << endl;
      break;
    case 3:
      cout << "Failure" << endl;
      break;
    case 4:
      cout << "Fatal" << endl;
      break;
    }
  }

  // move everything to a vector -- slow memory but okay for what we are doing
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (pafScanline[(i)*width + j] > 0)
        input[j][i] = pafScanline[(i) * width + j];
      else
        input[j][i] = 0;
    }
  }

  CPLFree(pafScanline);

  cout << "Done Loading" << endl;




  // Vectors for the data
  std::vector<GLM_Vertex> Vertices;
  std::vector<unsigned int> Indices;
  printf("xres: %.2f, yres: %.2f\n", xres, yres);

  // Time to construct a height map based on the xres and yres
  for (int i = 0; i < input.size() - 1; i++)
  {
    for (int j = 0; j < input[i].size() - 1; j++)
    {

      float UL = (float)(input[i][j]) / (float)(max); // Upper left
      float LL = (float)(input[i + 1][j]) / (float)(max); // Lower left
      float UR = (float)(input[i][j + 1]) / (float)(max); // Upper right
      float LR = (float)(input[i + 1][j + 1]) / (float)(max); // Lower right

      if (UL <= 0)
      {
        UL = 0;
      }
      if (UR <= 0)
      {
        UR = 0;
      }
      if (LR <= 0)
      {
        LR = 0;
      }
      if (LL <= 0)
      {
        LL = 0;
      }

      glm::vec3 ULV = glm::vec3(i * xres, UL * max, j * yres);
      glm::vec3 LLV = glm::vec3((i + 1)*xres, LL * max, j * yres);
      glm::vec3 URV = glm::vec3(i * xres, UR * max, (j + 1)*yres);
      glm::vec3 LRV = glm::vec3((i + 1)*xres, LR * max, (j + 1)*yres);

      // compute smoothed normal
      glm::vec3 a = ComputeNormal(ULV, i, j, input.size(), input[i].size(), input, max, xres, yres);
      glm::vec3 b = ComputeNormal(LLV, i + 1, j, input.size(), input[i].size(), input, max, xres, yres);
      glm::vec3 c = ComputeNormal(URV, i, j + 1, input.size(), input[i].size(), input, max, xres, yres);
      glm::vec3 d = ComputeNormal(LRV, i + 1, j + 1, input.size(), input[i].size(), input, max, xres, yres);

      Vertices.push_back(GLM_Vertex( glm::vec3(i * xres, UL, j * yres), 
                                glm::vec2( (float)i/(float)input.size(), (float)j/(float)input[i].size()), 
                                a, 
                                glm::vec3(0, 0, 0)));

      Vertices.push_back(GLM_Vertex( glm::vec3((i + 1)*xres, LL, j * yres),
                                glm::vec2((float)(i + 1) / (float)input.size(), (float)j / (float)input[i].size()),
                                b,  
                                glm::vec3(0, 0, 0)));

      Vertices.push_back(GLM_Vertex( glm::vec3(i * xres, UR, (j + 1)*yres),
                                glm::vec2((float)i / (float)input.size(), (float)(j + 1) / (float)input[i].size()),
                                c, 
                                glm::vec3(0, 0, 0)));
      Vertices.push_back(GLM_Vertex( glm::vec3((i + 1)*xres, LR, (j + 1)*yres), 
                                glm::vec2((float)(i + 1) / (float)input.size(), (float)(j + 1) / (float)input[i].size()),
                                d, 
                                glm::vec3(0, 0, 0)));


      Indices.push_back(Vertices.size() - 4);
      Indices.push_back(Vertices.size() - 1);
      Indices.push_back(Vertices.size() - 2);
      Indices.push_back(Vertices.size() - 4);
      Indices.push_back(Vertices.size() - 3);
      Indices.push_back(Vertices.size() - 1);
    }
  }

  size = Indices.size();
  // Add vertex and indices's buffer
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLM_Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  return true;
}

glm::vec3 Terrain::ComputeNormal(glm::vec3 center, int i, int j, int width, int height, vector<vector<float>>& data, float Max, float xres, float yres )
{
  // Compute center of all values which is the i and j passed in
  glm::vec3 left;
  glm::vec3 right;
  glm::vec3 up;
  glm::vec3 down;
  glm::vec3 sum = glm::vec3(0, 0, 0);
  bool l = false;
  bool r = false;
  bool u = false;
  bool d = false;

  int count = 0;
  // Compute left
  if (i - 1 >= 0)
  {

    left = glm::vec3((i - 1) * xres, data[i - 1][j], j * yres);
    left = center - left;
    l = true;
  }

  // Compute right
  if (i + 1 < width)
  {
    right = glm::vec3((i + 1) * xres, data[i + 1][j], j * yres);
    right = center - right;
    r = true;
  }

  // Compute up
  if (j - 1 >= 0)
  {
    up = glm::vec3((i) * xres, data[i][j - 1], (j - 1) * yres);
    up = center - up;
    u = true;
  }

  // Compute down
  if (j + 1 < height)
  {
    down = glm::vec3((i) * xres, data[i][j + 1], (j + 1) * yres);
    down = center - down;
    d = true;
  }

  // Compute normals
  if (u  && r)
  {
    glm::vec3 v1 = glm::cross(up, right);
    if (v1.y < 0)
    {
      v1 *= -1;
    }
    sum += v1;
    count = count + 1;
  }
  if (u && l)
  {
    glm::vec3 v1 = glm::cross(up, left);
    if (v1.y < 0)
    {
      v1 *= -1;
    }
    sum += v1;
    count = count + 1;
  }
  if (d && r)
  {
    glm::vec3 v1 = glm::cross(down, right);
    if (v1.y < 0)
    {
      v1 *= -1;
    }
    sum += v1;
    count = count + 1;
  }
  if (d && l)
  {
    glm::vec3 v1 = glm::cross(down, left);
    if (v1.y < 0)
    {
      v1 *= -1;
    }
    sum += v1;
    count = count + 1;
  }

  // Compute average normal
  sum /= count;
  auto t = glm::normalize(sum);


  // Normalize it and return :D!!!! Enjoy your smoothed normal for some smooth shading!
  return glm::normalize(sum);
};


void Terrain::ComputeGeoProperties(GDALDataset *poDataset, int width, int height, double& x, double& y, double& xright, double& ybottom, double& xres, double& yres)
{
  double adfGeoTransform[6];
  if ( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
  {
    printf( "Origin = (%.6f,%.6f)\n",
            adfGeoTransform[0], adfGeoTransform[3] );

    printf( "Pixel Size = (%.6f,%.6f)\n",
            adfGeoTransform[1], adfGeoTransform[5] );
    x = adfGeoTransform[0];
    y = adfGeoTransform[3];
    xright = x + adfGeoTransform[1] * (double)(width);
    ybottom = y + adfGeoTransform[5] * (double)(height);
  }
  else
  {
    return;
  }

  string proj;
  proj = string(poDataset->GetProjectionRef());

  OGRSpatialReference sr2;
  char* test = &proj[0];
  sr2.importFromWkt(&test);

  // Get Geography Coordinate System
  OGRSpatialReference* geog2 = sr2.CloneGeogCS();

  // Now to create coordinate transform function
  OGRCoordinateTransformation* poTransform2 = OGRCreateCoordinateTransformation( &sr2, geog2 );

  // Compute corners
  double x2=0;

  // Lets compute the resolution -- Despite the one provided by the geotransform
  double absoluteW = xright - x;
  double absoluteH = y - ybottom;

  // now lets compute the average resolution of the DEM
  xres = absoluteW / (width);
  yres = absoluteH / (height);

};

*/
