#include <billboard_technique.h>


BillboardTechnique::BillboardTechnique()
{   
}
 

bool BillboardTechnique::Initilize(string shader)
{
    if (!Technique::Init()) {
        return false;
    }

    string s = "../shaders/" + shader + "_vertex.glsl";
    if (!AddShader(GL_VERTEX_SHADER, s.c_str()))
    {
        printf("Error with Vertex in Billboard\n");
        return false;
    }

    s = "../shaders/" + shader + "_geometry.glsl";
    if (!AddShader(GL_GEOMETRY_SHADER, s.c_str()))
    {
        printf("Error with Geometry in Billboard\n");
        return false;
    }
    
    s = "../shaders/" + shader + "_fragment.glsl";
    if (!AddShader(GL_FRAGMENT_SHADER, s.c_str()))
    {
        printf("Error with Fragment in Billboard\n");
        return false;
    }

    if (!Finalize())
    {
        printf("Error Billboard Finalize\n");
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
    
    
void BillboardTechnique::SetVP(const glm::mat4 VP)
{
    glUniformMatrix4fv(m_VPLocation, 1, GL_FALSE, glm::value_ptr(VP));    
}

void BillboardTechnique::SetDelta(unsigned int dt)
{
    glUniform1f(m_DeltaTime, (float)dt);    
}

void BillboardTechnique::SetTime(int Time)
{
    glUniform1f(m_Time, (float)Time);    
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
