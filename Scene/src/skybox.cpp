/*

  Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <skybox.h>

SkyBox::SkyBox()
{
    m_pSkyboxTechnique = NULL;
    m_pCubemapTex = NULL;
    m_pMesh = NULL;
}


SkyBox::~SkyBox()
{
    SAFE_DELETE(m_pSkyboxTechnique);
    SAFE_DELETE(m_pCubemapTex);
    SAFE_DELETE(m_pMesh);
}


bool SkyBox::Init(const string& Directory,
                  const string& PosXFilename,
                  const string& NegXFilename,
                  const string& PosYFilename,
                  const string& NegYFilename,
                  const string& PosZFilename,
                  const string& NegZFilename)
{
    m_pSkyboxTechnique = new SkyboxTechnique();

    if (!m_pSkyboxTechnique->Init()) {
        printf("Error initializing the skybox technique\n");
        return false;
    }

    m_pSkyboxTechnique->Enable();
    m_pSkyboxTechnique->SetTextureUnit(0);
    
    m_pCubemapTex = new CubemapTexture(Directory,
                                       PosXFilename,
                                       NegXFilename,
                                       PosYFilename,
                                       NegYFilename,
                                       PosZFilename,
                                       NegZFilename);

    if (!m_pCubemapTex->Load()) {
        return false;
    }
        
    m_pMesh = new Mesh();

    return m_pMesh->LoadMesh("../content/sphere.obj"); 
}


void SkyBox::Render(glm::mat4 projection, glm::mat4 view, glm::mat4 camLoc)
{
  m_pSkyboxTechnique->Enable();
  

  GLint OldCullFaceMode;
  glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
  GLint OldDepthFuncMode;
  glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);
  
  glCullFace(GL_FRONT);
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);

  glm::mat4 model = camLoc * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
  glm::mat4 mvp = projection * view * model;
  m_pSkyboxTechnique->SetWVP(mvp);
  m_pCubemapTex->Bind(GL_TEXTURE0);
  m_pMesh->Render();  
  
  glCullFace(OldCullFaceMode);        
  glDepthFunc(OldDepthFuncMode);
  glDepthMask(GL_FALSE);  
}
