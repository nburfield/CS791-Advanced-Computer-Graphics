
#include <limits.h>
#include <string.h>

#include "water_technique.h"
#include "ogldev_util.h"

WaterTechnique::WaterTechnique()
{   
}

bool WaterTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "../shaders/water_vertex.glsl")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "../shaders/water_fragment.glsl")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    /*
    m_WVPLocation = GetUniformLocation("gWVP");

  if (m_WVPLocation == INVALID_UNIFORM_LOCATION) {
    return false;
  }
  */

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    size = 100;

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            Vertex v(Vector3f(i, 2, j),
                     Vector2f(0, 0),
                     Vector3f(0, 0, 0),
                     Vector3f(0, 0, 0));
        
            Vertices.push_back(v);
        }
    }

    for(int i = 0; i < (size * size) - size; i++)
    {
        if(i % size == 0)
        {
            Indices.push_back(i);
            Indices.push_back(i + 1);
            Indices.push_back(i + size);
        }
        else
        {
            Indices.push_back(i);
            Indices.push_back(i + size);
            Indices.push_back(i + size - 1);
            Indices.push_back(i);
            Indices.push_back(i + 1);
            Indices.push_back(i + size);
        }
    }

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    texture = new Texture(GL_TEXTURE_2D, "../Content/bricks.jpg");
  
    if (!texture->Load())
    {
        return false;
    }

  return true;
}

void WaterTechnique::Render(const Matrix4f& VP, const Vector3f& CameraPos)
{
    Enable();

    //SetWVP(VP);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);                 // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); // texture coordinate
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); // normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32); // tangent

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    texture->Bind(COLOR_TEXTURE_UNIT);

    glDrawElements(GL_TRIANGLES, size * size, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

}


void WaterTechnique::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);    
}

void WaterTechnique::SetModel(const Matrix4f& WVP)
{
    model = WVP;    
}

