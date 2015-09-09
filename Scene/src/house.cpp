#include <house.h>

House::House()
{
  model = glm::mat4(1.0f);
}


House::~House()
{

}


bool House::Initilize(std::string file)
{
  if (!Technique::Init())
  {
    return false;
  }

  if (!AddShader(GL_VERTEX_SHADER, "../shaders/house_vertex.glsl"))
  {
    return false;
  }

  if (!AddShader(GL_FRAGMENT_SHADER, "../shaders/house_fragment.glsl"))
  {
    return false;
  }

  if (!Finalize())
  {
    return false;
  }

  m_WVPLocation = GetUniformLocation("mvpMatrix");
  if (m_WVPLocation == INVALID_UNIFORM_LOCATION)
  {
    return false;
  }

  if(!object.LoadMesh(file))
  {
    return false;
  }
  return true;
}


void House::Render(glm::vec3 loc, glm::mat4 view, glm::mat4 proj)
{
  Enable();
  
  glm::mat4 mvp = proj * view * (model * glm::translate(glm::mat4(1.0f), loc));
  glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));
  object.Render();
}


void House::Rotate(float dt)
{
  spin += dt;
  model = glm::rotate(glm::mat4(1.0f), spin, glm::vec3(0.0, 1.0, 0.0));
}
