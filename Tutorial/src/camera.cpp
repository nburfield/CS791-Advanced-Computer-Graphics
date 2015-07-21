#include <camera.h>

// Constructor
Camera::Camera()
{
  // Assign the initial camer locations
  //0,5,12 << Previous Default Values

  x_view_update = y_view_update = z_view_update = 0;
  x_view_next = x_view = 0.0;
  y_view_next = y_view = 0.0;
  z_view_next = z_view = 0.0;
  x_view = -40.0;
  y_view = -30.0;
  z_view = 20.0;
  x_focus_next = x_focus = 0.0;
  y_focus_next = y_focus = 0.0;
  z_focus_next = z_focus = 1.0;


  mode = 0;
  defaultT_f = threshold_f = 400;
  defaultT_v = threshold_v = 100;
  objX = 0;
  objY = 0;
  objZ = 0;

  // Projection View never changes
  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 4.0f/3.0f, //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane,
}

// Destructor
Camera::~Camera()
{

}

// Return the updated view of the game
glm::mat4 Camera::getView()
{
  //Check if focus should be changed
  if(!inFocus())
  {
    slowChange(x_focus, x_focus_next, threshold_f);
    slowChange(y_focus, y_focus_next, threshold_f);
    slowChange(z_focus, z_focus_next, threshold_f);
  }

  // Update the view
  slowChange(x_view, x_view_next, threshold_v);
  slowChange(y_view, y_view_next, threshold_v);
  slowChange(z_view, z_view_next, threshold_v);

  // Return the new view
  return glm::lookAt( glm::vec3((x_view + x_view_update), (y_view + y_view_update), (z_view + z_view_update)),
                      glm::vec3(x_focus, y_focus, z_focus),
                      glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Camera::getProjection()
{
  return projection;
}


// Move the camera to a preset location
void Camera::viewUpdate()
   {
    // Switch on a mode to get the camera view
  switch(mode)
     {
    case 0:
      x_view_update = y_view_update = z_view_update = 0;
      x_view_next = 20.0;
      y_view_next = 30.0;
      z_view_next = 70.0;
      x_focus_next = x_focus = 10.0;
      y_focus_next = y_focus = 1.0;
      z_focus_next = z_focus = -5.0;
        break;
    case 1:
      x_focus = 0.0;
      y_focus = 1.0;
      z_focus =  0.0;
      x_view_update = 0.0;
      y_view_update = 0.0;
      z_view_update = 0.0;
      x_view = 0.0;
      y_view = 45.0;
      z_view = 0.0;
      break;
    case 2:
      x_view_update = x_view_next = x_view = 0.0;
      y_view_update = y_view_next = y_view = 5.0;
      z_view_update = z_view_next = z_view = 12.0;
      x_focus_next = x_focus = 0.0;
      y_focus_next = y_focus = 0.0;
      z_focus_next = z_focus = 0.0;
      break;
    case 3: //follow
      x_focus_next = objX;
      y_focus_next = objY;
      z_focus_next = objZ;
      x_view_update = 0.0;
      y_view_update = 0.0;
      z_view_update = 0.0;
      x_view_next = objX - 15;
      y_view_next = objY + 45;
      z_view_next = objZ + 15;
      break;

    case 4: //follow
      x_focus_next = 0;
      y_focus_next = 0;
      z_focus_next = 0;
      x_view_update = 0.0;
      y_view_update = 0.0;
      z_view_update = 0.0;
      x_view_next = objX;
      y_view_next = objY;
      z_view_next = objZ;
      break;
     }
   }

void Camera::setMode(int newMode)
{
  mode = newMode;
}

void Camera::setObj(float x, float y, float z)
{
  objX = x;
  objY = y;
  objZ = z;
}

void Camera::setThresh(float vw, float foc)
{
  threshold_v = vw;
  threshold_f = foc;
}

void Camera::moveHorizontal(float val)
{
  //X-Locations
  float next = x_view_update += val;
  if(next > -60.0 && next < 60.0)
  {
    x_view_update += val;
  }
}

void Camera::moveVerticle(float val)
{
  //Y-Locations
  float next = y_view_update += val;
  if(next > -60.0 && next < 60.0)
  {
    y_view_update += val;
  }
}

void Camera::moveLateral(float val)
{
  //Z-Locations
  float next = z_view_update += val;
  if(next > -60.0 && next < 60.0)
  {
    z_view_update += val;
  }
}

void Camera::focusHorizontal(float val)
{
  //X-Focus
  x_focus_next += val;
}

void Camera::focusVerticle(float val)
{
  //Y-Focus
  y_focus_next += val;
}

void Camera::focusLateral(float val)
{
  //Z-Focus
  z_focus_next += val;
}

// Slowly moves the camera
void Camera::slowChange(float &current, float next, float divisor)
{
  float change = ((next-current)/divisor);
  current+=change;
}

// Checks if the camera is focused
bool Camera::inFocus()
   {
    //Check X
  if((x_focus_next - x_focus) != 0)
     {
      return false;
     }

    // Check Y
  if((y_focus_next - y_focus) != 0)
     {
      return false;
     }

    // Check Z
  if((z_focus_next - z_focus) != 0)
     {
      return false;
     }

    // No errors return true
    return true;
   }

void Camera::print()
{
  printf("Location(%f, %f, %f) - Focus(%f, %f, %f)", (x_view + x_view_update), (y_view + y_view_update), (z_view + z_view_update), x_focus, y_focus, z_focus);
}

float Camera::x()
  { return x_view + x_view_update; }
float Camera::y()
  { return y_view + y_view_update; }
float Camera::z()
  { return z_view + z_view_update; }

glm::vec3 Camera::getFocus()
{
  return glm::vec3(x_focus, y_focus, z_focus);
}

float Camera::nextX()
  { return x_view_next; }
float Camera::nextY()
  { return y_view_next; }
float Camera::nextZ()
  { return z_view_next; }

/* OLD
Camera::Camera()
{
  // Projection View never changes
  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 4.0f/3.0f, //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane,

  // Initialize the values that will calculate the camera view
  direction = glm::vec3(0, 0, 1);
  motion = glm::vec3(0, 0, 0);
  position = glm::vec3(0, 0, -10);
  up = glm::vec3(0, 1, 0);

  // Translations
  foreward = side = flight = 0;
  angleSpeedX = angleSpeedY = 70;
  angleX = angleY = 0;
  motionSpeed = 100;

  // Min and Max Values
  maxAngleX = maxAngleY = 75;
  maxForeward = maxSide = maxFlight = 10;
  minForeward = minFlight = minSide = -10;
}

glm::mat4 Camera::getProjection()
{
  return projection;
}

glm::mat4 Camera::getView()
{
  direction = glm::rotate(direction, angleX, up);
  direction = glm::rotate(direction, angleY, glm::cross(direction, up));
  motion += foreward * direction;
  motion += side * glm::cross(up, direction);
  motion += flight * up;
  position += motion;
  view = glm::lookAt( position, //Eye Position
                      position + direction, //Focus point
                      up); //Positive Y is up
  motion = glm::vec3(0, 0, 0);
  reset();
  return view;
}

void Camera::rotateX(float dir)
{
  angleX = dir * angleSpeedX;
}

void Camera::rotateY(float dir)
{
  angleY = dir * angleSpeedY;
}

void Camera::setTranslate(float dir)
{
  foreward = motionSpeed * dir;
  foreward = glm::clamp(foreward, minForeward, maxForeward);
}

void Camera::setStrafe(float dir)
{
  side = motionSpeed * dir;
  side = glm::clamp(side, minSide, maxSide);
}

void Camera::setFlight(float dir)
{
  flight = motionSpeed * dir;
  flight = glm::clamp(flight, minFlight, maxFlight);
}

void Camera::orbitX(float dir)
{
  angleX += dir * angleSpeedX;
  glm::vec3 d = glm::rotate(direction, angleX, up);
  d = glm::rotate(d, angleY, glm::cross(d, up));
  if(dir > 0)
  {
    motion -= motionSpeed * glm::cross(up, glm::normalize(d));
  }
  else
  {
    motion += motionSpeed * glm::cross(up, d);
  }
}

void Camera::reset()
{
  foreward = side = flight = angleX = angleY = 0;
}
*/ 
