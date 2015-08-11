#include <geom_pass.h>

GeomPass::GeomPass()
{

}

GeomPass::~GeomPass()
{

}

bool GeomPass::Init()
{
  Program::Init();
  // Start Shaders
  Shader vertex, fragment;
  if(!vertex.Initilize("../shader/geometry_pass_vertex.glsl", GL_VERTEX_SHADER, program))
  {
    return false;
  }
  std::cout<<"Geometry Vertex Shader Success"<<std::endl;
  if(!fragment.Initilize("../shader/geometry_pass_fragment.glsl", GL_FRAGMENT_SHADER, program))
  {
    return false;
  }
  std::cout<<"Geometry Fragment Shader Success"<<std::endl;

  glLinkProgram(program);

  //check if everything linked ok
  GLint shader_status;
  glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
  if(!shader_status)
  {
    std::cout << "Geometry THE SHADER PROGRAM FAILED TO LINK" << std::endl;
    return false;
  }

  // Get uniform locations of the objects
  mvp = glGetUniformLocation(program, const_cast<const char*>("gWVP"));
  if(mvp == -1)
  {
    printf("gWVP NOT FOUND\n");
    return false;
  }

  model = glGetUniformLocation(program, const_cast<const char*>("gWorld"));
  if(model == -1)
  {
    printf("gWorld NOT FOUND\n");
    return false;
  }

  texture = glGetUniformLocation(program, const_cast<const char*>("gColorMap"));
  if(texture == -1)
  {
    printf("ColorMap NOT FOUND\n");
    return false;
  }

  // Everything passed
  return true;
}

void GeomPass::SetMVP(glm::mat4 wvp)
{
  glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(wvp));
}

void GeomPass::SetModel(glm::mat4 Model)
{
  glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));
}

void GeomPass::SetTexture(unsigned int TextureUnit)
{
  glUniform1i(texture, TextureUnit);
}

