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

#ifndef BILLBOARD_TECHNIQUE_H
#define BILLBOARD_TECHNIQUE_H

#include <programHandle.h>
#include <iostream>
#include <opengl_data.h>

class BillboardTechnique : public Program 
{
public:
    
    BillboardTechnique();
 
    virtual bool Init();
    
    void SetVP(const glm::mat4 VP);
    void SetCameraPosition(const glm::vec3 Pos);
    void SetColorTextureUnit(unsigned int TextureUnit);
    void SetBillboardSize(float BillboardSize);
    
private:

    GLuint m_VPLocation;
    GLuint m_cameraPosLocation;
    GLuint m_colorMapLocation;
    GLuint m_billboardSizeLocation;
};

#endif  /* BILLBOARD_TECHNIQUE_H */

