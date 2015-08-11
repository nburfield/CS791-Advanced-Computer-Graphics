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

#include <ps_update.h>

PSUpdateTechnique::PSUpdateTechnique()
{
    
}
    
    
bool PSUpdateTechnique::Init()
{
  if (!Program::Init()) {
      return false;
  }

  // Start Shaders
  Shader vertex, geometry, fragment;
  if(!vertex.Initilize("../shader/ps_update_vertex.glsl", GL_VERTEX_SHADER, program))
  {
    return false;
  }
  std::cout<<"Particle System Vertex Shader Success"<<std::endl;
  if(!geometry.Initilize("../shader/ps_update_geometry.glsl", GL_GEOMETRY_SHADER, program))
  {
    return false;
  }
  std::cout<<"Particle System Geometry Shader Success"<<std::endl;
  if(!fragment.Initilize("../shader/ps_update_fragment.glsl", GL_FRAGMENT_SHADER, program))
  {
    return false;
  }
  std::cout<<"Particle System Fragment Shader Success"<<std::endl;

  glLinkProgram(program);

  //check if everything linked ok
  GLint shader_status;
  glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
  if(!shader_status)
  {
    std::cout << "Particle System  THE SHADER PROGRAM FAILED TO LINK" << std::endl;
    return false;
  }

  const GLchar* Varyings[4];    
  Varyings[0] = "Type1";
  Varyings[1] = "Position1";
  Varyings[2] = "Velocity1";    
  Varyings[3] = "Age1";
  
  glTransformFeedbackVaryings(program, 4, Varyings, GL_INTERLEAVED_ATTRIBS);

  glLinkProgram(program);

  //check if everything linked ok
  glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
  if(!shader_status)
  {
    std::cout << "Particle System  THE SHADER PROGRAM FAILED TO LINK" << std::endl;
    return false;
  }

  // Get uniform locations of the objects    
  m_deltaTimeMillisLocation = glGetUniformLocation(program, const_cast<const char*>("gDeltaTimeMillis"));
  if(m_deltaTimeMillisLocation == -1)
  {
    printf("m_deltaTimeMillisLocation not found\n");
    return false;
  }

    m_randomTextureLocation = glGetUniformLocation(program, const_cast<const char*>("gRandomTexture"));
  if(m_randomTextureLocation == -1)
  {
    printf("m_randomTextureLocation not found\n");
    return false;
  }

    m_timeLocation = glGetUniformLocation(program, const_cast<const char*>("gTime"));
  if(m_timeLocation == -1)
  {
    printf("m_timeLocation not found\n");
    return false;
  }

    m_launcherLifetimeLocation = glGetUniformLocation(program, const_cast<const char*>("gLauncherLifetime"));
  if(m_launcherLifetimeLocation == -1)
  {
    printf("m_launcherLifetimeLocation not found\n");
    return false;
  }

    m_shellLifetimeLocation = glGetUniformLocation(program, const_cast<const char*>("gShellLifetime"));
  if(m_shellLifetimeLocation == -1)
  {
    printf("m_shellLifetimeLocation not found\n");
    return false;
  }

    m_secondaryShellLifetimeLocation = glGetUniformLocation(program, const_cast<const char*>("gSecondaryShellLifetime"));
  if(m_secondaryShellLifetimeLocation == -1)
  {
    printf("m_secondaryShellLifetimeLocation not found\n");
    return false;
  }
    
  return true;
}


void PSUpdateTechnique::SetDeltaTimeMillis(unsigned int DeltaTimeMillis)
{
    glUniform1f(m_deltaTimeMillisLocation, (float)DeltaTimeMillis);
}


void PSUpdateTechnique::SetTime(int Time)
{
    glUniform1f(m_timeLocation, (float)Time);
}


void PSUpdateTechnique::SetRandomTextureUnit(unsigned int TextureUnit)
{    
    glUniform1i(m_randomTextureLocation, TextureUnit);
}


void PSUpdateTechnique::SetLauncherLifetime(float Lifetime)
{
    glUniform1f(m_launcherLifetimeLocation, Lifetime);
}


void PSUpdateTechnique::SetShellLifetime(float Lifetime)
{
    glUniform1f(m_shellLifetimeLocation, Lifetime);
}


void PSUpdateTechnique::SetSecondaryShellLifetime(float Lifetime)
{
    glUniform1f(m_secondaryShellLifetimeLocation, Lifetime);
}
