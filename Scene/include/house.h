#include <opengl_data.h>
#include <mesh.h>
#include <string>
#include <technique.h>
#include <lights_common.h>

class House : public Technique
{
  public:
    House(glm::vec3 loc);
    ~House();
    bool Initilize(std::string file);
    void Render(glm::mat4 view, glm::mat4 proj);
    void Rotate(float dt);
    void Move(float step);
    void SetLight();

  private:
    Mesh object;
    glm::mat4 model;
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
