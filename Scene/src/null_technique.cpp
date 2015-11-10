
#include <null_technique.h>

NullTechnique::NullTechnique()
{   
}

bool NullTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "../shaders/null_technique_vertex.glsl")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "../shaders/null_technique_fragment.glsl")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");

  if (m_WVPLocation == INVALID_UNIFORM_LOCATION) {
    return false;
  }

  return true;
}


void NullTechnique::SetWVP(const glm::mat4 WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(WVP));    
}

