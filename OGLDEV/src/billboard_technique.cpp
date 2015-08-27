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

#include "billboard_technique.h"
#include "ogldev_util.h"




BillboardTechnique::BillboardTechnique()
{   
}
 

bool BillboardTechnique::Init(string shader)
{
    if (!Technique::Init()) {
        return false;
    }

    string s = "../shaders/" + shader + "_vertex.glsl";
    if (!AddShader(GL_VERTEX_SHADER, s.c_str())) {
        return false;
    }

    s = "../shaders/" + shader + "_geometry.glsl";
    if (!AddShader(GL_GEOMETRY_SHADER, s.c_str())) {
        return false;
    }
    
    s = "../shaders/" + shader + "_fragment.glsl";
    if (!AddShader(GL_FRAGMENT_SHADER, s.c_str())) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_VPLocation = GetUniformLocation("gVP");
    m_cameraPosLocation = GetUniformLocation("gCameraPos");
    m_colorMapLocation = GetUniformLocation("gColorMap");
    m_billboardSizeLocation = GetUniformLocation("gBillboardSize");
    m_DeltaTime = GetUniformLocation("gDT"); 
    m_Time = GetUniformLocation("gTime");

    if (m_VPLocation == INVALID_UNIFORM_LOCATION ||
        m_cameraPosLocation == INVALID_UNIFORM_LOCATION ||
        m_billboardSizeLocation == INVALID_UNIFORM_LOCATION ||
        m_colorMapLocation == INVALID_UNIFORM_LOCATION ||
        m_DeltaTime == INVALID_UNIFORM_LOCATION ||
        m_Time == INVALID_UNIFORM_LOCATION) {
        return false;        
    }
    
    return GLCheckError();
}
    
    
void BillboardTechnique::SetVP(const Matrix4f& VP)
{
    glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, (const GLfloat*)VP.m);    
}

void BillboardTechnique::SetDelta(unsigned int dt)
{
    glUniform1f(m_DeltaTime, (float)dt);    
}

void BillboardTechnique::SetTime(int Time)
{
    glUniform1f(m_Time, (float)Time);    
}

void BillboardTechnique::SetCameraPosition(const Vector3f& Pos)
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
