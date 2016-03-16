#include <opengl_data.h>
#include <mesh.h>
#include <string>
#include <technique.h>
#include <lights_common.h>

class House : public Technique
{
  public:
    House(glm::vec3 loc, glm::vec3 loc2, glm::vec3 loc3);
    ~House();
    bool Initilize(std::string file);
    void Render(glm::mat4 view, glm::mat4 proj);
    void Rotate(float dt);
    void Move(float step);
    void SetLight();

    glm::mat4 model_house;
    glm::mat4 model_light1;
    glm::mat4 model_light2;
  private:
    Mesh object;
    Mesh light1;
    Mesh light2;
    glm::vec3 location;
    float spin;

    PointLight Sun;

    struct
    {
      GLuint Color;
      GLuint AmbientIntensity;
      GLuint DiffuseIntensity;
      GLuint Position;
      struct
      {
        GLuint Constant;
        GLuint Linear;
        GLuint Exp;
      } Atten;
    } SunLight;

    GLint ProjMatrix;
    GLint ViewMatrix;
    GLint ModelMatrix;
    GLint NormalMatrix;
};
