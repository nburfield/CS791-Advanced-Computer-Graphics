/*
    Built to handle the camera in an opengl world
*/
#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

class Camera
{
  public:
    Camera();
    ~Camera();
    glm::mat4 getProjection();
    glm::mat4 getView();

    /* Old
    void rotateX(float dir);
    void rotateY(float dir);
    void setTranslate(float dir);
    void setStrafe(float dir);
    void setFlight(float dir);
    void orbitX(float dir);
    void reset();
    */
    void viewUpdate();
    void setMode(int newMode);
    void setObj(float x, float y, float z);
    void setThresh(float vw, float foc);
    float x();
    float y();
    float z();
    float nextX();
    float nextY();
    float nextZ();
    void moveHorizontal(float val);
    void moveVerticle(float val);
    void moveLateral(float val);
    void focusHorizontal(float val);
    void focusVerticle(float val);
    void focusLateral(float val);
    void print();
    glm::vec3 getFocus();

  private:
    /* Old
    glm::mat4 view;
    
    glm::vec3 direction;
    glm::vec3 motion;
    glm::vec3 position;
    glm::vec3 up;

    // Translations
    float foreward, side, flight, angleSpeedX, angleSpeedY, angleX, angleY, motionSpeed;

    // Min and Max Values
    float maxAngleX, maxAngleY, maxForeward, maxSide, maxFlight;
    float minForeward, minSide, minFlight;
    */
    glm::mat4 projection;
    void slowChange(float &current, float next, float divisor);
    bool inFocus();

    float x_view,
          y_view,
          z_view,
          x_view_next,
          y_view_next,
          z_view_next,
          x_view_update,
          y_view_update,
          z_view_update,
          x_focus,
          y_focus,
          z_focus,
          x_focus_next,
          y_focus_next,
          z_focus_next,
          objX,
          objY,
          objZ;

    int mode;
    float threshold_f,
        threshold_v,
        defaultT_f,
        defaultT_v;
};
#endif
