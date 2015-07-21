#include <object.h>

Object::Object()
{
  on = true;
  position = glm::vec3(0.0, 0.0, 0.0);
}

Object::Object(glm::vec3 pos)
{
  on = true;
  position = glm::vec3(pos.x, pos.y, pos.x);
  model = glm::translate(glm::mat4(1.0f), position);
}

void Object::toggle()
{
  on = !on;
}

bool Object::ison()
{
  return on;
}

glm::vec3 Object::getPosition()
{
  return position;
}

void Object::setPosition(glm::vec3 pos)
{
  position = pos;
  model = glm::translate(glm::mat4(1.0f), position);
}
