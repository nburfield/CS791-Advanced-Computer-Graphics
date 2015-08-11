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

#include <limits.h>
#include <string.h>

#include <skybox_tech.h>


SkyboxTechnique::SkyboxTechnique()
{   
}

bool SkyboxTechnique::Init()
{
  if (!Program::Init()) 
  {
    return false;
  }

  Shader vertex, fragment;
  if(!vertex.Initilize("../shader/vertSkybox.glsl", GL_VERTEX_SHADER, program))
  {
    return false;
  }
  std::cout<<"Skybox Vertex Shader Success"<<std::endl;
  if(!fragment.Initilize("../shader/fragmentSkybox.glsl", GL_FRAGMENT_SHADER, program))
  {
    return false;
  }
  std::cout<<"Skybox Fragment Shader Success"<<std::endl;

  glLinkProgram(program);

  //check if everything linked ok
  GLint shader_status;
  glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
  if(!shader_status)
  {
    std::cout << "Skybox THE SHADER PROGRAM FAILED TO LINK" << std::endl;
    return false;
  }

  m_WVPLocation = glGetUniformLocation(program, const_cast<const char*>("gWVP"));
  if(m_WVPLocation == -1)
  {
    printf("gWVP NOT FOUND\n");
    return false;
  }
    
  m_textureLocation = glGetUniformLocation(program, const_cast<const char*>("gCubemapTexture"));
  if(m_textureLocation == -1)
  {
    printf("gCubemapTexture NOT FOUND\n");
    return false;
  }

  return true;
}


void SkyboxTechnique::SetWVP(glm::mat4 WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(WVP));    
}


void SkyboxTechnique::SetTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_textureLocation, TextureUnit);
}


