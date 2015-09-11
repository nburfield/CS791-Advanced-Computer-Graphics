// Includes
#include <opengl_data.h>
#include <SDL2/SDL.h>
#include <window.h>
#include <sys/time.h>
#include "assert.h"
#include <house.h>
#include <water_technique.h>
#include <terrain_technique.h>
using namespace chrono;

// Window
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600


// Functions
bool Initilize();
void Update();
void Render();
void close();
bool Keyboard(SDL_Event e, float dt);
unsigned int getDT();
float getdt();
long long GetCurrentTimeMillis();
std::string ErrorString(GLenum error);

// Global Variables
Window *window;
Camera *camera;
House *house;
WaterTechnique *water;
Terrain *terrain;

//Time function
float getdt();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;
long long m_currentTimeMillis;
high_resolution_clock::time_point current;

// Main
int main()
{
  // Init the screen
  if(!Initilize())
  {
    printf("Failed to startup\n");
    return 1;
  }

  current = high_resolution_clock::now();
  high_resolution_clock::time_point past = high_resolution_clock::now();

  SDL_Event e;
  bool run = true;
  // Startup
  while(run)
  {
    current = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(current - past);

    
    while(SDL_PollEvent(&e) != 0)
    {
      run = Keyboard(e, time_span.count());
    }

    Update();
    Render();
    past = current;
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
  house = new House(glm::vec3(38.22, 73.09, -30.20));
  if(!house->Initilize("../content/house.3ds"))
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

  // Load the terrain
  terrain = new Terrain(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(500.0f, 100.0f, 500.0f));
  if(!terrain->Initilize("../content/output.jpg")) //heightmap.bmp, island-height.jpg
  {
    printf("The Terrain did not load.\n");
    return false;
  }

  // Return Success
  return true;
}


void Update()
{
  camera->update();
}


void Render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor( 0.f, 0.f, 0.5f, 0.f );
  float dt = getdt();

  water->Render(glm::vec3(58.22, 73.09, -30.20), camera->GetView(), camera->GetProjection());
  // house->Rotate(dt);
  house->Render(camera->GetView(), camera->GetProjection());

  terrain->Render(camera->GetView(), camera->GetProjection(), dt);

  // Get the Error
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }

  // Swap to the Window
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

bool Keyboard(SDL_Event e, float dt)
{
  if(e.type == SDL_QUIT)
  {
    return false;
  }
  else if (e.type == SDL_KEYDOWN)
  {
    // handle key down events here
    if (e.key.keysym.sym == SDLK_ESCAPE)
    {
      return false;
    }

    // rotate camera left
    if (e.key.keysym.sym == SDLK_q)
    {
      camera->rotateX(1 * dt);
    }

    // rotate camera right
    if (e.key.keysym.sym == SDLK_e)
    {
      camera->rotateX(-1 * dt);
    }

    //Camera.applyRotation();

    // Move left
    if (e.key.keysym.sym == SDLK_a)
    {
      camera->strafe(1 * dt);
    }
    // move back
    if (e.key.keysym.sym == SDLK_s)
    {
      camera->translate(-5 * dt);
    }

    // move right
    if (e.key.keysym.sym == SDLK_d)
    {
      camera->strafe(-1 * dt);
    }

    // move forward
    if (e.key.keysym.sym == SDLK_w)
    {
      camera->translate(1 * dt);
    }

    if (e.key.keysym.sym == SDLK_z)
    {
      camera->rotateY(-1 * dt);
    }
    if (e.key.keysym.sym == SDLK_x)
    {
      camera->rotateY(1 * dt);
    }
    if (e.key.keysym.sym == SDLK_r)
    {
      camera->flight(1 * dt);
    }
    if (e.key.keysym.sym == SDLK_f)
    {
      camera->flight(-1 * dt);
    }
    if(e.key.keysym.sym == SDLK_p)
    {
      printf("The Camera Position: (%.2f, %.2f, %.2f)\n", camera->getPos().x, camera->getPos().y, camera->getPos().z);
      printf("The Camera Focus: (%.2f, %.2f, %.2f)\n", camera->getFocus().x, camera->getFocus().y, camera->getFocus().z);
    }
    if (e.key.keysym.sym == SDLK_y)
    {
      house->Move(1.0);
    }
    if (e.key.keysym.sym == SDLK_u)
    {
      house->Move(-1.0);
    }
  }
  else if (e.type == SDL_KEYUP)
  {
    if (e.key.keysym.sym == SDLK_w)
    {
      camera->resetVerticalSpeed();
    }
    else if (e.key.keysym.sym == SDLK_s)
    {
      camera->resetVerticalSpeed();
    }
    if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_d)
    {
      camera->resetHorizontalSpeed();
    }
    if (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_e)
    {
      // Reset Horizontal rotation
      camera->resetHorizontalRotation();
    }
    if (e.key.keysym.sym == SDLK_z || e.key.keysym.sym == SDLK_x)
    {
      camera->resetVerticalRotation();
    }
    if (e.key.keysym.sym == SDLK_r || e.key.keysym.sym == SDLK_f)
    {
      camera->resetFlightSpeed();
    }
  
  }

  // Return Success
  return true;
}
