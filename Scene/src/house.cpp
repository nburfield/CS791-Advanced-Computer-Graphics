#include <house.h>

House::House(glm::vec3 loc)
{
  location = loc;
  model = glm::translate(glm::mat4(1.0f), loc);

  Sun.AmbientIntensity = 0.02f;
  Sun.DiffuseIntensity = 0.2f;
  Sun.Color = glm::vec3(0.0f, 1.0f, 0.0f);
  Sun.Position = glm::vec3(-18.60f, 242.30f, -298.37f);
  Sun.Attenuation.Constant = 0.0f;
  Sun.Attenuation.Linear = 0.0f;
  Sun.Attenuation.Exp = 0.03f;
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

  SunLight.Color = GetUniformLocation("sunLight.Base.Color");
  if(SunLight.Color == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.Color not found.\n");
    return false;
  }
  SunLight.AmbientIntensity = GetUniformLocation("sunLight.Base.AmbientIntensity");
  if(SunLight.AmbientIntensity == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.AmbientIntensity not found.\n");
    return false;
  }
  SunLight.DiffuseIntensity = GetUniformLocation("sunLight.Base.DiffuseIntensity");
  if(SunLight.DiffuseIntensity == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.DiffuseIntensity not found.\n");
    return false;
  }
  SunLight.Position = GetUniformLocation("sunLight.Position");
  if(SunLight.Position == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.Position not found.\n");
    return false;
  }
  SunLight.Atten.Constant = GetUniformLocation("sunLight.Atten.Constant");
  if(SunLight.Atten.Constant == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.Atten.Constant not found.\n");
    return false;
  }
  SunLight.Atten.Linear = GetUniformLocation("sunLight.Atten.Linear");
  if(SunLight.Atten.Linear == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.Atten.Linear not found.\n");
    return false;
  }
  SunLight.Atten.Exp = GetUniformLocation("sunLight.Atten.Exp");
  if(SunLight.Atten.Exp == INVALID_UNIFORM_LOCATION)
  {
    printf("SunLight.Atten.Exp not found.\n");
    return false;
  }
  ProjMatrix = GetUniformLocation("matrices.projMatrix");
  if(ProjMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("ProjMatrix not found\n");
    return false;
  }
  ViewMatrix = GetUniformLocation("matrices.viewMatrix");
  if(ViewMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("ViewMatrix not found\n");
    return false;
  }
  ModelMatrix = GetUniformLocation("matrices.modelMatrix");
  if(ModelMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("ModelMatrix not found\n");
    return false;
  }
  NormalMatrix = GetUniformLocation("matrices.normalMatrix");
  if(NormalMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("NormalMatrix not found\n");
    return false;
  }

  if(!object.LoadMesh(file))
  {
    return false;
  }
  return true;
}


void House::Render(glm::mat4 view, glm::mat4 proj)
{
  Enable();
  SetLight();

  glUniformMatrix4fv(ProjMatrix, 1, GL_FALSE, glm::value_ptr(proj));
  glUniformMatrix4fv(ViewMatrix, 1, GL_FALSE, glm::value_ptr(view));  
  glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(model));  
  glUniformMatrix4fv(NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

  object.Render();
}



void House::SetLight()
{
  //Sun.Position = glm::vec3(500.0 * cos(spin), 500.0 * sin(spin), 0.0);

  glUniform3fv(SunLight.Color, 1, glm::value_ptr(Sun.Color));
  glUniform1f(SunLight.AmbientIntensity, Sun.AmbientIntensity);
  glUniform1f(SunLight.DiffuseIntensity, Sun.DiffuseIntensity);
  glUniform3fv(SunLight.Position, 1, glm::value_ptr(Sun.Position));
  glUniform1f(SunLight.Atten.Constant, Sun.Attenuation.Constant);
  glUniform1f(SunLight.Atten.Linear, Sun.Attenuation.Linear);
  glUniform1f(SunLight.Atten.Exp, Sun.Attenuation.Exp);
}


void House::Rotate(float dt)
{
  spin += dt * M_PI;
  //model = glm::rotate(model, spin, glm::vec3(0.0, 1.0, 0.0));
  model = glm::translate( glm::mat4(1.0f), glm::vec3(500.0 * cos(spin), 500.0 * sin(spin), 0.0));
}


void House::Move(float step)
{
  location.y += step;
  model = glm::translate( glm::mat4(1.0f), location);
  printf("Location: %.2f, %.2f, %.2f\n", location.x, location.y, location.z);
}
