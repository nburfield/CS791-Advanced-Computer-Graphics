#include <programHandle.h>

bool Program::Init()
{
  program = glCreateProgram();
  return true;
}

bool Program::Start()
{
  glUseProgram(program);
  return true;
}
