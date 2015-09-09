// Includes
#include <opengl_data.h>
#include <window.h>
#include <sys/time.h>
#include "assert.h"
#include <house.h>
#include <water_technique.h>

// Window
#define WINDOW_WIDTH  840
#define WINDOW_HEIGHT 680

// Functions
bool Initilize();
bool Update();
void Render();
void close();
unsigned int getDT();
float getdt();
long long GetCurrentTimeMillis();
std::string ErrorString(GLenum error);

// Global Variables
Window *window;
Camera *camera;
House *house;
WaterTechnique *water;

//Time function
float getdt();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;
long long m_currentTimeMillis;

// Main
int main()
{
  // Init the screen
  if(!Initilize())
  {
    printf("Failed to startup\n");
    return 1;
  }

  bool run = true;
  // Startup
  while(run)
  {
    run = Update();
    Render();
  }

  // End
  close();
  return 0;
}

bool Initilize()
{
  // Init Window
  window = new Window();
  if(!window->Initilize("Nolan Scene", WINDOW_HEIGHT, WINDOW_WIDTH))
  {
    printf("Window Failed to Initialize\n");
    return false;
  }

  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();
    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  m_currentTimeMillis = GetCurrentTimeMillis();
  t1 = std::chrono::high_resolution_clock::now();

  // Init Camera
  camera = new Camera();
  if(!camera->Initilize())
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Load the House
  house = new House();
  if(!house->Initilize("../content/house.obj"))
  {
    printf("The House did not load.\n");
    return false;
  }

  // Load the Water
  water = new WaterTechnique();
  if(!water->Initilize())
  {
    printf("The Water did not load.\n");
    return false;
  }

  // Return Success
  return true;
}


bool Update()
{
  // Keyboard Input
  if(!window->Update(camera))
  {
    return false;
  }

  // Return success
  return true;
}


void Render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  water->Render(glm::vec3(0.0, 0.0, 0.0), camera->GetView(), camera->GetProjection());
  // house->Rotate(getdt());
  house->Render(glm::vec3(0.0, -10.0, 40.0), camera->GetView(), camera->GetProjection());
  window->Swap();
}

void close()
{
  delete window;
  window = NULL;
}

unsigned int getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

float getdt()
{
  float ret;
  t2 = std::chrono::high_resolution_clock::now();
  ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
  t1 = std::chrono::high_resolution_clock::now();
  return ret;
}


std::string ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

long long GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
