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

#include <directional_pass.h>


DSDirLightPassTech::DSDirLightPassTech()
{   
}

bool DSDirLightPassTech::Init()
{
  if (!Program::Init()) 
  {
    return false;
  }

  Shader vertex, fragment;
  if(!vertex.Initilize("../shader/vertLightPass.glsl", GL_VERTEX_SHADER, program))
  {
    return false;
  }
  std::cout<<"DirectionalLight Vertex Shader Success"<<std::endl;
  if(!fragment.Initilize("../shader/fragmentDirect.glsl", GL_FRAGMENT_SHADER, program))
  {
    return false;
  }
  std::cout<<"DirectionalLight Fragment Shader Success"<<std::endl;

  glLinkProgram(program);

  //check if everything linked ok
  GLint shader_status;
  glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
  if(!shader_status)
  {
    std::cout << "DirectionalLight THE SHADER PROGRAM FAILED TO LINK" << std::endl;
    return false;
  }
    
  m_dirLightLocation.Color = glGetUniformLocation(program, const_cast<const char*>("gDirectionalLight.Base.Color"));
  if(m_dirLightLocation.Color == -1)
  {
    printf("m_dirLightLocation.Color NOT FOUND\n");
    return false;
  }
  m_dirLightLocation.AmbientIntensity = glGetUniformLocation(program, const_cast<const char*>("gDirectionalLight.Base.AmbientIntensity"));
  if(m_dirLightLocation.AmbientIntensity == -1)
  {
    printf("m_dirLightLocation.AmbientIntensity NOT FOUND\n");
    return false;
  }
  m_dirLightLocation.Direction = glGetUniformLocation(program, const_cast<const char*>("gDirectionalLight.Direction"));
  if(m_dirLightLocation.Direction == -1)
  {
    printf("m_dirLightLocation.Direction NOT FOUND\n");
    return false;
  }
  m_dirLightLocation.DiffuseIntensity = glGetUniformLocation(program, const_cast<const char*>("gDirectionalLight.Base.DiffuseIntensity"));
  if(m_dirLightLocation.DiffuseIntensity == -1)
  {
    printf("m_dirLightLocation.DiffuseIntensity NOT FOUND\n");
    return false;
  }

  return DSLightPassTech::Init();
}



void DSDirLightPassTech::SetDirectionalLight(const DirectionalLight& Light)
{
    glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
    glm::vec3 Direction = Light.Direction;
    //Direction = glm::normalize(Direction);
    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}


