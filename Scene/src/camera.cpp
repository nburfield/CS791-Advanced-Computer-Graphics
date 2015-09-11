#include <camera.h>

Camera::Camera()
{
  view = glm::lookAt( glm::vec3(-18.60, 242.30, -298.37), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, -1.0, 0.0)); //Positive Y is up
  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 4.0f/3.0f, //Aspect Ratio, so Circles stay Circular
                                 0.9f, //Distance to the near plane, normally a small value like this
                                 100000.0f); //Distance to the far plane, 


  maxAngleX = 15;
  maxAngleY = 15;
  angularSpeedX = 2;
  angularSpeedY = 2;
  direction = glm::vec3(-61.61, -97.77, -15.10);
  position = glm::vec3(57.75, 98.44, -27.59);
  //direction = glm::vec3(0, 1, 0);
  motionvector = glm::vec3(0, 0, 0);
  //position = glm::vec3(0, 10, -16);
  angleX = 0;
  angleY = 0;
  up = glm::vec3(0, 1, 0);
  motionSpeed = 10;

  update();

  maxForwardVel = 6;
  minForwardVel = -6;
  maxSideVel = 10;
  minSideVel = -10;
  maxFlightVel = 10;
  minFlightVel = -10;
  sideVel = flightVel = forwardVel = 0;
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


void Camera::orbitX(float dir)
{

    angleX += dir * angularSpeedX;
    glm::vec3 d = glm::rotate(direction, angleX, up);
    d = glm::rotate(d, angleY, glm::cross(d, up));
    if (dir > 0)
        motionvector -= motionSpeed * glm::cross(up, glm::normalize(d));
    else
        motionvector += motionSpeed * glm::cross(up, d);
}

void Camera::rotateX(float dir)
{
    angleX = dir * angularSpeedX;
}

void Camera::rotateY(float dir)
{
    angleY = dir * angularSpeedY;
}
void Camera::applyRotation()
{
    direction = glm::rotate(direction, angleX, up);
    direction = glm::rotate(direction, angleY, glm::cross(direction, up));
}

void Camera::translate(float dir)
{
    forwardVel = motionSpeed * dir;
    forwardVel = glm::clamp(forwardVel, minForwardVel, maxForwardVel);
}

void Camera::strafe(float dir)
{
    sideVel = motionSpeed * dir;
    sideVel = glm::clamp(sideVel, minSideVel, maxSideVel);
}

void Camera::update()
{
    applyRotation();
    motionvector += forwardVel * direction;
    motionvector += sideVel * glm::cross(up, direction);
    motionvector += flightVel * up;
    position += motionvector;

    view = glm::lookAt( position, //Eye Position
                        position + direction, //Focus point
                        up); //Positive Y is up
    
    motionvector = glm::vec3(0, 0, 0);
}

void Camera::resetHorizontalSpeed()
{
    sideVel = 0;
}

void Camera::resetVerticalSpeed()
{
    forwardVel = 0;
}

void Camera::resetHorizontalRotation()
{
    angleX = 0;
}

void Camera::resetVerticalRotation()
{
    angleY = 0;
}

void Camera::flight(float dir)
{
    flightVel = motionSpeed *dir;
}

void Camera::resetFlightSpeed()
{
    flightVel = 0;
}
