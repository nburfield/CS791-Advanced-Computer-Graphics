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

#include <light_pass.h>


DSLightPassTech::DSLightPassTech()
{   
}

bool DSLightPassTech::Init()
{
  m_WVPLocation = glGetUniformLocation(program, const_cast<const char*>("mvpMatrix"));
  if(m_WVPLocation == -1)
  {
    printf("m_WVPLocation NOT FOUND\n");
    return false;
  }
  m_posTextureUnitLocation = glGetUniformLocation(program, const_cast<const char*>("gPositionMap"));
  if(m_posTextureUnitLocation == -1)
  {
    printf("m_posTextureUnitLocation NOT FOUND\n");
    return false;
  }
  m_colorTextureUnitLocation = glGetUniformLocation(program, const_cast<const char*>("myTextureSampler"));
  if(m_colorTextureUnitLocation == -1)
  {
    printf("m_colorTextureUnitLocation NOT FOUND\n");
    return false;
  }
  m_normalTextureUnitLocation = glGetUniformLocation(program, const_cast<const char*>("gNormalMap"));
  if(m_normalTextureUnitLocation == -1)
  {
    printf("m_normalTextureUnitLocation NOT FOUND\n");
    return false;
  }
  m_eyeWorldPosLocation = glGetUniformLocation(program, const_cast<const char*>("gEyeWorldPos"));
  if(m_eyeWorldPosLocation == -1)
  {
    printf("m_eyeWorldPosLocation NOT FOUND\n");
    return false;
  }
  m_matSpecularIntensityLocation = glGetUniformLocation(program, const_cast<const char*>("gMatSpecularIntensity"));
  if(m_matSpecularIntensityLocation == -1)
  {
    printf("m_matSpecularIntensityLocation NOT FOUND\n");
    return false;
  }
  m_matSpecularPowerLocation = glGetUniformLocation(program, const_cast<const char*>("gSpecularPower"));
  if(m_matSpecularPowerLocation == -1)
  {
    printf("m_matSpecularPowerLocation NOT FOUND\n");
    return false;
  }
  m_screenSizeLocation = glGetUniformLocation(program, const_cast<const char*>("gScreenSize"));
  if(m_screenSizeLocation == -1)
  {
    printf("m_screenSizeLocation NOT FOUND\n");
    return false;
  }

  return true;
}


void DSLightPassTech::SetWVP(glm::mat4 WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(WVP));    
}


void DSLightPassTech::SetPositionTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_posTextureUnitLocation, TextureUnit);
}


void DSLightPassTech::SetColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_colorTextureUnitLocation, TextureUnit);
}


void DSLightPassTech::SetNormalTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_normalTextureUnitLocation, TextureUnit);
}


void DSLightPassTech::SetEyeWorldPos(glm::vec3 EyePos)
{
  glUniform3f(m_eyeWorldPosLocation, EyePos.x, EyePos.y, EyePos.z);
}


void DSLightPassTech::SetScreenSize(unsigned int Width, unsigned int Height)
{
    glUniform2f(m_screenSizeLocation, (float)Width, (float)Height);
}
