#include <opengl_data.h>

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initilize();
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void translate(float dir);
    void strafe(float dir);
    void flight(float dir);
    void resetHorizontalSpeed();
    void resetVerticalSpeed();
    void resetHorizontalRotation();
    void resetVerticalRotation();
    void resetFlightSpeed();
    void orbitX(float dir);
    void rotateX(float dir);
    void rotateY(float dir);
    void applyRotation();
    void update();
    glm::vec3 getPos()
    {
        return position;
    };
    glm::vec3 getFocus()
    {
      return direction;
    };
  
  private:
    glm::mat4 view;
    glm::mat4 projection;

    float maxAngleX;
    float maxAngleY;
    float angleX;
    float angleY;
    float angularSpeedX;
    float angularSpeedY;
    float motionSpeed;

    // translation speed
    float forwardVel, sideVel, flightVel;
    float maxForwardVel, minForwardVel, maxSideVel, minSideVel,
          maxFlightVel, minFlightVel;

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 motionvector;
};
