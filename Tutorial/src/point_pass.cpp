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

#include "point_pass.h"


DSPointLightPassTech::DSPointLightPassTech()
{   
}

bool DSPointLightPassTech::Init()
{
    if (!Program::Init()) {
        return false;
    }

  Shader vertex, fragment;
  if(!vertex.Initilize("../shader/vertLightPass.glsl", GL_VERTEX_SHADER, program))
  {
    return false;
  }
  std::cout<<"PointLight Vertex Shader Success"<<std::endl;
  if(!fragment.Initilize("../shader/fragmentPoint.glsl", GL_FRAGMENT_SHADER, program))
  {
    return false;
  }
  std::cout<<"PointLight Fragment Shader Success"<<std::endl;

  glLinkProgram(program);

  //check if everything linked ok
  GLint shader_status;
  glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
  if(!shader_status)
  {
    std::cout << "PointLight THE SHADER PROGRAM FAILED TO LINK" << std::endl;
    return false;
  }
   
    m_pointLightLocation.Color = glGetUniformLocation(program, const_cast<const char*>("gPointLight.Base.Color"));
    if(m_pointLightLocation.Color == -1)
    {
      printf("gPointLight.Base.Color NOT FOUND\n");
      return false;
    }
    m_pointLightLocation.AmbientIntensity = glGetUniformLocation(program, const_cast<const char*>("gPointLight.Base.AmbientIntensity"));
    if(m_pointLightLocation.AmbientIntensity == -1)
    {
      printf("gPointLight.Base.AmbientIntensity NOT FOUND\n");
      return false;
    }
    m_pointLightLocation.Position = glGetUniformLocation(program, const_cast<const char*>("gPointLight.Position"));
    if(m_pointLightLocation.Position == -1)
    {
      printf("gPointLight.Position NOT FOUND\n");
      return false;
    }
    m_pointLightLocation.DiffuseIntensity = glGetUniformLocation(program, const_cast<const char*>("gPointLight.Base.DiffuseIntensity"));
    if(m_pointLightLocation.DiffuseIntensity == -1)
    {
      printf("gPointLight.Base.DiffuseIntensity NOT FOUND\n");
      return false;
    }
    m_pointLightLocation.Atten.Constant = glGetUniformLocation(program, const_cast<const char*>("gPointLight.Atten.Constant"));
    if(m_pointLightLocation.Atten.Constant == -1)
    {
      printf("gPointLight.Atten.Constant NOT FOUND\n");
      return false;
    }
    m_pointLightLocation.Atten.Linear = glGetUniformLocation(program, const_cast<const char*>("gPointLight.Atten.Linear"));
    if(m_pointLightLocation.Atten.Linear == -1)
    {
      printf("gPointLight.Atten.Linear NOT FOUND\n");
      return false;
    }
    m_pointLightLocation.Atten.Exp = glGetUniformLocation(program, const_cast<const char*>("gPointLight.Atten.Exp"));
    if(m_pointLightLocation.Atten.Exp == -1)
    {
      printf("gPointLight.Atten.Exp NOT FOUND\n");
      return false;
    }

  return DSLightPassTech::Init();
}


void DSPointLightPassTech::SetPointLight(const PointLight& Light)
{
    glUniform3f(m_pointLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_pointLightLocation.AmbientIntensity, Light.AmbientIntensity);
    glUniform1f(m_pointLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
    glUniform3f(m_pointLightLocation.Position, Light.Position.x, Light.Position.y, Light.Position.z);
    glUniform1f(m_pointLightLocation.Atten.Constant, Light.Attenuation.Constant);
    glUniform1f(m_pointLightLocation.Atten.Linear, Light.Attenuation.Linear);
    glUniform1f(m_pointLightLocation.Atten.Exp, Light.Attenuation.Exp);
}


