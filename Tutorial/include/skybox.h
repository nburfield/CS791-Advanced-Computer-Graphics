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

#ifndef SKYBOX_H
#define SKYBOX_H

#include <skybox_tech.h>
#include <cube_map_texture.h>
#include <mesh2.h>

#include <opengl_data.h>

class SkyBox
{
public:
    SkyBox();

    ~SkyBox();
    
    bool Init(const string& Directory,
              const string& PosXFilename,
              const string& NegXFilename,
              const string& PosYFilename,
              const string& NegYFilename,
              const string& PosZFilename,
              const string& NegZFilename);
    
    void Render(glm::mat4 projection, glm::mat4 view, glm::mat4 camLoc);
    
private:    
    SkyboxTechnique* m_pSkyboxTechnique;
    CubemapTexture* m_pCubemapTex;
    BasicMesh* m_pMesh;
};

#endif  /* SKYBOX_H */

