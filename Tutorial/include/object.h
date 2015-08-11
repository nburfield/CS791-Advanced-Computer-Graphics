/* 
  Used for holing data on a rendered object
*/

#ifndef OBJECT_H
#define OBJECT_H

//#include <basic_mesh.h>
#include <mesh2.h>

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object : public BasicMesh
{
  public:
    Object();
    Object(glm::vec3 pos);
    void toggle();
    bool ison();
    glm::vec3 getPosition();
    void setPosition(glm::vec3 pos);

    glm::mat4 model;
    
  private:
    bool on;
    glm::vec3 position;
    
};

#endif
