#include <opengl_data.h>
#include <mesh.h>
#include <string>
#include <technique.h>

class House : public Technique
{
  public:
    House();
    ~House();
    bool Initilize(std::string file);
    void Render(glm::vec3 loc, glm::mat4 view, glm::mat4 proj);
    void Rotate(float dt);

  private:
    Mesh object;
    GLuint m_WVPLocation;
    glm::mat4 model;
    float spin;
};
