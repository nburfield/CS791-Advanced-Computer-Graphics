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

#include <billboard_technique.h>


BillboardTechnique::BillboardTechnique()
{   
}
 

bool BillboardTechnique::Init()
{

  if (!Program::Init()) {
      return false;
  }

  // Start Shaders
  Shader vertex, geometry, fragment;
  if(!vertex.Initilize("../shader/billboard_vertex.glsl", GL_VERTEX_SHADER, program))
  {
    return false;
  }
  std::cout<<"Billboard Vertex Shader Success"<<std::endl;
  if(!geometry.Initilize("../shader/billboard_geometry.glsl", GL_GEOMETRY_SHADER, program))
  {
    return false;
  }
  std::cout<<"Billboard Geometry Shader Success"<<std::endl;
  if(!fragment.Initilize("../shader/billboard_fragment.glsl", GL_FRAGMENT_SHADER, program))
  {
    return false;
  }
  std::cout<<"Billboard Fragment Shader Success"<<std::endl;

  glLinkProgram(program);

  //check if everything linked ok
  GLint shader_status;
  glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
  if(!shader_status)
  {
    std::cout << "Particle System  THE SHADER PROGRAM FAILED TO LINK" << std::endl;
    return false;
  }

  m_VPLocation = glGetUniformLocation(program, const_cast<const char*>("gVP"));
  if(m_VPLocation == -1)
  {
    printf("m_VPLocation not found\n");
    return false;
  }

  m_cameraPosLocation = glGetUniformLocation(program, const_cast<const char*>("gCameraPos"));
  if(m_cameraPosLocation == -1)
  {
    printf("m_cameraPosLocation not found\n");
    return false;
  }

  m_colorMapLocation = glGetUniformLocation(program, const_cast<const char*>("gColorMap"));
  if(m_colorMapLocation == -1)
  {
    printf("m_colorMapLocation not found\n");
    return false;
  }

  m_billboardSizeLocation = glGetUniformLocation(program, const_cast<const char*>("gBillboardSize"));
  if(m_billboardSizeLocation == -1)
  {
    printf("m_billboardSizeLocation not found\n");
    return false;
  }

  
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    std::string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
    return false;
  }
  
  
  return true;
}
    
    
void BillboardTechnique::SetVP(const glm::mat4 VP)
{
    glUniformMatrix4fv(m_VPLocation, 1, GL_FALSE, glm::value_ptr(VP));    
}


void BillboardTechnique::SetCameraPosition(const glm::vec3 Pos)
{
    glUniform3f(m_cameraPosLocation, Pos.x, Pos.y, Pos.z);
}


void BillboardTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_colorMapLocation, TextureUnit);
}


void BillboardTechnique::SetBillboardSize(float BillboardSize)
{
    glUniform1f(m_billboardSizeLocation, BillboardSize);
}
