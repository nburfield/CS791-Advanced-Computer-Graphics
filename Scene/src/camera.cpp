#include <camera.h>

Camera::Camera()
{
  view = glm::lookAt( glm::vec3(0.0, 8.0, -16.0), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 4.0f/3.0f, //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
}


Camera::~Camera()
{

}


bool Camera::Initilize()
{

  // Return Success
  return true;
}


glm::mat4 Camera::GetProjection()
{
  return projection;
}


glm::mat4 Camera::GetView()
{
  return view;
}
