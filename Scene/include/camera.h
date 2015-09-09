#include <opengl_data.h>

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initilize();
    glm::mat4 GetProjection();
    glm::mat4 GetView();
  
  private:
    glm::mat4 view;
    glm::mat4 projection;
};
