#include <ps_update_technique.h>

PSUpdateTechnique::PSUpdateTechnique()
{
    
}
    
    
bool PSUpdateTechnique::Initilize()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "../shaders/particle_system_vertex.glsl")) {
        return false;
    }

    if (!AddShader(GL_GEOMETRY_SHADER, "../shaders/particle_system_geometry.glsl")) {
        return false;
    }

  if (!AddShader(GL_FRAGMENT_SHADER, "../shaders/particle_system_fragment.glsl")) {
    return false;
  }

  if (!Finalize()) {
    return false;
  }

    const GLchar* Varyings[5];    
    Varyings[0] = "Type1";
    Varyings[1] = "Position1";
    Varyings[2] = "Velocity1";  
    Varyings[3] = "Age1";
    Varyings[4] = "Color1";
    
    glTransformFeedbackVaryings(m_shaderProg, 5, Varyings, GL_INTERLEAVED_ATTRIBS);

    if (!Finalize()) {
        return false;
    }
    
    m_deltaTimeMillisLocation = GetUniformLocation("gDeltaTimeMillis");
    m_randomTextureLocation = GetUniformLocation("gRandomTexture");
    m_timeLocation = GetUniformLocation("gTime");
    m_launcherLifetimeLocation = GetUniformLocation("gLauncherLifetime");
    m_shellLifetimeLocation = GetUniformLocation("gShellLifetime");
    m_secondaryShellLifetimeLocation = GetUniformLocation("gSecondaryShellLifetime");
    m_newShellColor = GetUniformLocation("newColor");

    if (m_deltaTimeMillisLocation == INVALID_UNIFORM_LOCATION ||
        m_timeLocation == INVALID_UNIFORM_LOCATION ||
        m_randomTextureLocation == INVALID_UNIFORM_LOCATION ||
        m_launcherLifetimeLocation == INVALID_UNIFORM_LOCATION ||
        m_shellLifetimeLocation == INVALID_UNIFORM_LOCATION ||
        m_secondaryShellLifetimeLocation == INVALID_UNIFORM_LOCATION ||
        m_newShellColor == INVALID_UNIFORM_LOCATION) {
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

void PSUpdateTechnique::SetRandomColor()
{   
    glm::vec3 color = glm::normalize(glm::vec3(RandomFloat(), RandomFloat(), RandomFloat()));
    glUniform3f(m_newShellColor, color.x, color.y, color.z);
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
