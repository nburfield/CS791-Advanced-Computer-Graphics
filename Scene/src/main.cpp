// Includes
#include <opengl_data.h>
#include <SDL2/SDL.h>
#include <window.h>
#include <sys/time.h>
#include <assert.h>
#include <house.h>
#include <water_technique.h>
#include <terrain_technique.h>
#include <flag_technique.h>
#include <triangle.h>
#include <particle.h>
#include <billboard_list.h>
#include <lights_common.h>
#include <gbuffer.h>
#include <ds_geom_pass_tech.h>
#include <ds_dir_light_pass_tech.h>
#include <ds_point_light_pass_tech.h>
#include <null_technique.h>
#include <skybox.h>
#include <chrono>

using namespace chrono;


// Window
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;


// Functions
bool Initilize(char *filename);
void Update();
void Render();
void close();
bool Keyboard(SDL_Event e, float dt);
unsigned int getDT();
float getdt();
long long GetCurrentTimeMillis();
std::string ErrorString(GLenum error);

// Used for object placement
float x = -84.78, y = 59.36, z = 2.06;
int selectMoveItem = 0;
const unsigned int numPointLights = 10;
float speed = 0.1f;

// Global Variables
Window *window;
Camera *camera;
House *house;
WaterTechnique *water;
FlagTechnique *flag;
Terrain *terrain;
Triangle *triangle;
ParticleSystem *fireworks;
BillboardList *grass;
SkyBox* skyBox;

// Deferred Shading & Lights
GBuffer gbuffer;
DSGeomPassTech GeomPass;
DSDirLightPassTech DirLightPass;
DSPointLightPassTech PointLightPass;
NullTechnique NullTech;
PointLight pointLight[numPointLights];
Mesh sphere;
Mesh cone;
DirectionalLight DirLight;

// Functions Related to DS
void InitLights();
void GeometryPass();
void DirectionalLightPass();
void FinalPass();
float CalcPointLightBSphere(const PointLight& Light);
void DSStencilPass(unsigned int PointLightIndex);
void DSPointLightPass(unsigned int PointLightIndex);

//Time function
float getdt();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;
long long m_currentTimeMillis;
high_resolution_clock::time_point current;

unsigned int DT;


// Main
int main(int argc, char **argv)
{
  // Init the screen
  //if(argc < 2)
  //{
  //  printf("Does not have the correct arguments. Need Heightmap\n");
  //  return 1;
  //}

  if(!Initilize(argv[1]))
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

bool Initilize(char *filename)
{
  // Init Window
  window = new Window();
  if(!window->Initilize("Nolan Scene", &WINDOW_HEIGHT, &WINDOW_WIDTH))
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
  house = new House(glm::vec3(-107.76, 57.87, 4.54), glm::vec3(-5.10, 57.87, -49.80), glm::vec3(-8.51, 57.87, -7.37));
  if(!house->Initilize("../content/House/3ds_file.3DS")) // ../content/streetlamp/streetlamp.obj
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

  // Load the flag
  flag = new FlagTechnique();
  if(!flag->Initilize())
  {
    printf("The Flag did not load.\n");
    return false;
  }

  // Load the Water
  triangle = new Triangle();
  if(!triangle->Initilize())
  {
    printf("The Triangle did not load.\n");
    return false;
  }

  // Skybox
  skyBox = new SkyBox();
  if (!skyBox->Init("../content/", "bluesky_right.jpg", "bluesky_left.jpg", "bluesky_top.jpg", 
                       "bluesky_top.jpg", "bluesky_front.jpg", "bluesky_back.jpg")) 
  {
    printf("Skybox Failed to init.\n");
    return false;
  }

  // Load the terrain
  terrain = new Terrain(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 200.0f, 1000.0f));
  if(!terrain->Initilize("../content/TahoeBasin.jpg")) //heightmap.bmp, island-height.jpg
  {
    printf("The Terrain did not load.\n");
    return false;
  }

  grass = new BillboardList();
  if (!grass->Initilize("../content/grass.jpg", terrain->GrassVertices, terrain->vRenderScale))
  {
    printf("Failed to startup billboard.\n");
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////////////////

  // Init the lights
  InitLights();

  // Setup the gbuffer
  if(!gbuffer.Init(WINDOW_WIDTH, WINDOW_HEIGHT))
  {
    std::cout<<"GBuffer Init failed.\n";
    return false;
  }
  
  if(!GeomPass.Init())
  {
    std::cout<<"GPass Init failed.\n";
    return false;
  }

  GeomPass.Enable();
  GeomPass.SetColorTextureUnit(5);
  
  if (!PointLightPass.Init()) 
  {
    printf("Error initializing DSPointLightPassTech\n");
    return false;
  }

  PointLightPass.Enable();
  PointLightPass.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  PointLightPass.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  PointLightPass.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  PointLightPass.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  if(!NullTech.Init())
  {
    printf("Null Tech Failed\n");
    return false;
  }


  if (!DirLightPass.Init())
  {
    printf("Error initializing DSDirLightPassTech\n");
    return false;
  }

  DirLightPass.Enable();
  DirLightPass.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  DirLightPass.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  DirLightPass.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  DirLightPass.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  if(!sphere.LoadMesh("../content/sphere.obj"))
  {
    printf("Failed to load the sphere.\n");
    return false;
  }

  if(!cone.LoadMesh("../content/cone.obj"))
  {
    printf("Failed to load the cone.\n");
    return false;
  }

  //////////////////////////////////////////////////////////////////////////////////////////

  // Return Success
  return true;
}

void InitLights()
{
  /*
  SpotLight.AmbientIntensity = 00.5f;
  SpotLight.DiffuseIntensity = 0000.9f;
  SpotLight.Color = COLOR_WHITE;
  SpotLight.Attenuation.Linear = 0.01f;
  SpotLight.Position  = Vector3f(-20.0, 20.0, 5.0f);
  SpotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
  SpotLight.Cutoff =  20.0f;
  */

  DirLight.AmbientIntensity = 0.5f;
  DirLight.Color = COLOR_WHITE;
  DirLight.DiffuseIntensity = 0.5f;
  DirLight.Direction = glm::vec3(25.87, -64.91, 93.2);

  pointLight[0].DiffuseIntensity = 0.6f;
  pointLight[0].Color = COLOR_GREEN;
  pointLight[0].Position = glm::vec3(15.51, 59.73, 49.37);
  pointLight[0].Attenuation.Constant = 1.8f;
  pointLight[0].Attenuation.Linear = 0.8f;
  pointLight[0].Attenuation.Exp = 0.3f;
  pointLight[0].on = false;

  pointLight[1].DiffuseIntensity = 100.6f;
  pointLight[1].Color = COLOR_WHITE;
  pointLight[1].Position = glm::vec3(-5.10, 59.73, -49.80);
  pointLight[1].Attenuation.Constant = 1.0f;
  pointLight[1].Attenuation.Linear = 1.8f;
  pointLight[1].Attenuation.Exp = 1.3f;
  pointLight[1].on = false;

  pointLight[2].DiffuseIntensity = 5.2f;
  pointLight[2].Color = COLOR_RED;
  pointLight[2].Position = glm::vec3(-90.41, 71.90, 9.39);
  pointLight[2].Attenuation.Constant = 0.5f;
  pointLight[2].Attenuation.Linear = 0.5f;        
  pointLight[2].Attenuation.Exp = 0.3f;
  pointLight[2].on = false;

  pointLight[3].DiffuseIntensity = 5.6f;
  pointLight[3].Color = COLOR_YELLOW;
  pointLight[3].Position = glm::vec3(-93.80, 67.13, 21.08);
  pointLight[3].Attenuation.Constant = 0.3f;
  pointLight[3].Attenuation.Linear = 0.3f;
  pointLight[3].Attenuation.Exp = 0.3f;
  pointLight[3].on = false;

  pointLight[4].DiffuseIntensity = 100.2f;
  pointLight[4].Color = COLOR_WHITE;
  pointLight[4].Position = glm::vec3(-8.51, 62.27, -7.37);
  pointLight[4].Attenuation.Constant = 1.0f;
  pointLight[4].Attenuation.Linear = 1.0f;
  pointLight[4].Attenuation.Exp = 1.3f;
  pointLight[4].on = false;

  pointLight[5].DiffuseIntensity = 50.2f;
  pointLight[5].Color = COLOR_GREEN;
  pointLight[5].Position = glm::vec3(78.15, 69.13, -218.24);
  pointLight[5].Attenuation.Constant = 1.3f;
  pointLight[5].Attenuation.Linear = 1.3f;        
  pointLight[5].Attenuation.Exp = 0.3f;
  pointLight[5].on = false;

  pointLight[6].DiffuseIntensity = 5.2f;
  pointLight[6].Color = COLOR_RED;
  pointLight[6].Position = glm::vec3(-112.34, 67.47, 33.17);
  pointLight[6].Attenuation.Constant = 0.5f;
  pointLight[6].Attenuation.Linear = 0.5f;        
  pointLight[6].Attenuation.Exp = 0.3f;
  pointLight[6].on = false;

  pointLight[7].DiffuseIntensity = 5.2f;
  pointLight[7].Color = COLOR_RED;
  pointLight[7].Position = glm::vec3(-133.68, 74.60, 0.62);
  pointLight[7].Attenuation.Constant = 0.5f;
  pointLight[7].Attenuation.Linear = 0.5f;        
  pointLight[7].Attenuation.Exp = 0.3f;
  pointLight[7].on = false;

  pointLight[8].DiffuseIntensity = 5.2f;
  pointLight[8].Color = COLOR_RED;
  pointLight[8].Position = glm::vec3(-129.65, 70.87, -21.60);
  pointLight[8].Attenuation.Constant = 0.5f;
  pointLight[8].Attenuation.Linear = 0.5f;        
  pointLight[8].Attenuation.Exp = 0.3f;
  pointLight[8].on = false;

  pointLight[9].DiffuseIntensity = 5.2f;
  pointLight[9].Color = COLOR_RED;
  pointLight[9].Position = glm::vec3(-110.80, 70.77, -22.03);
  pointLight[9].Attenuation.Constant = 0.5f;
  pointLight[9].Attenuation.Linear = 0.5f;        
  pointLight[9].Attenuation.Exp = 0.3f;
  pointLight[9].on = false;
}

void Update()
{
  DT = getDT();
  camera->update();
  pointLight[0].Position = glm::vec3(x, y, z);
}

float CalcPointLightBSphere(const PointLight& Light)
{
  float MaxChannel = fmax(fmax(Light.Color.x, Light.Color.y), Light.Color.z);
  
  float ret = (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 
               * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel 
               * Light.DiffuseIntensity))) / 2 * Light.Attenuation.Exp;
  
  return ret;
}  

void GeometryPass()
{
  GeomPass.Enable();

  gbuffer.BindForGeomPass();

  // Only the geometry pass updates the depth buffer
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // Render Stuff
  //GeomPass.SetWVP( camera->GetProjection() * camera->GetView() * house->model);
  //GeomPass.SetWorldMatrix(camera->GetView());
  house->Render(camera->GetView(), camera->GetProjection());

  flag->Render(glm::vec3(-107.74, 86.13, -5.04), camera->GetView(), camera->GetProjection());
  water->Render(glm::vec3(275.72, 35.04, 660.75), camera->GetView(), camera->GetProjection());

  grass->Render(DT, camera->getPos() + camera->getFocus(), camera->GetProjection() * camera->GetView(), camera->GetView());

  float dt = getdt();
  terrain->Render(camera->GetView(), camera->GetProjection(), dt);

  // When we get here the depth buffer is already populated and the stencil pass
  // depends on it, but it does not write to it.
  glDepthMask(GL_FALSE);    
}

void DSStencilPass(unsigned int PointLightIndex)
{
  NullTech.Enable();

  // Disable color/depth write and enable stencil
  gbuffer.BindForStencilPass();
  glEnable(GL_DEPTH_TEST);

  glDisable(GL_CULL_FACE);

  glClear(GL_STENCIL_BUFFER_BIT);

  // We need the stencil test to be enabled but we want it
  // to succeed always. Only the depth test matters.
  glStencilFunc(GL_ALWAYS, 0, 0);

  glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
  glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

  // Set the lights, and the positions
  float BBoxScale = CalcPointLightBSphere(pointLight[PointLightIndex]);        
            
  NullTech.SetWVP(camera->GetProjection() * camera->GetView() * 
                        glm::translate(glm::mat4(1.0f), pointLight[PointLightIndex].Position) * 
                        glm::scale(glm::mat4(1.0f), glm::vec3(BBoxScale, BBoxScale, BBoxScale)));


  sphere.Render();  
}

    
void DSPointLightPass(unsigned int PointLightIndex)
{
  gbuffer.BindForLightPass();

  PointLightPass.Enable();
  
  PointLightPass.SetEyeWorldPos(camera->getPos());        

  glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
  
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  
  // Set the lights, and the positions
  float BBoxScale = CalcPointLightBSphere(pointLight[PointLightIndex]);        
            
  PointLightPass.SetWVP(camera->GetProjection() * camera->GetView() * 
                        glm::translate(glm::mat4(1.0f), pointLight[PointLightIndex].Position) * 
                        glm::scale(glm::mat4(1.0f), glm::vec3(BBoxScale, BBoxScale, BBoxScale)));

  PointLightPass.SetPointLight(pointLight[PointLightIndex]);
  sphere.Render(); 
  
  // Set back the GL
  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}


void DirectionalLightPass()
{
  gbuffer.BindForLightPass();

  DirLightPass.Enable();
  DirLightPass.SetDirectionalLight(DirLight);
  DirLightPass.SetEyeWorldPos(camera->getPos());

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);
            
  //float BBoxScale = CalcPointLightBSphere(pointLight[0]);

  DirLightPass.SetWVP(camera->GetProjection() * camera->GetView() * 
                      glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f)));


  //DirLightPass.SetWVP(camera->GetProjection() * camera->GetView() * 
  //                    glm::translate(glm::mat4(1.0f), glm::vec3(-98.62, 71.01, 25.68)) * 
  //                    glm::scale(glm::mat4(1.0f), glm::vec3(BBoxScale, BBoxScale, BBoxScale)));
  
  sphere.Render();

  glDisable(GL_BLEND);
}


void FinalPass()
{
  gbuffer.BindForFinalPass();
  glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 
                    0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void Render()
{  
  gbuffer.StartFrame();
  GeometryPass();

  // We need stencil to be enabled in the stencil pass to get the stencil buffer
  // updated and we also need it in the light pass because we render the light
  // only if the stencil passes.
  glEnable(GL_STENCIL_TEST);

  for (unsigned int i = 0 ; i < numPointLights; i++)
  {
    if(pointLight[i].on)
    {
      DSStencilPass(i);
      DSPointLightPass(i);
    }
  }

  // The directional light does not need a stencil test because its volume
  // is unlimited and the final pass simply copies the texture.
  glDisable(GL_STENCIL_TEST);

  DirectionalLightPass();

  triangle->Render(glm::vec3(0, 0, 0), camera->GetView(), camera->GetProjection());

  skyBox->Render(camera->GetProjection(), camera->GetView(), 
                 glm::translate(glm::mat4(1.0f), camera->getPos()));


  if(fireworks != NULL)
  {
    fireworks->Render(DT, camera->getPos() + camera->getFocus(), camera->GetProjection() * camera->GetView());    
  }

  FinalPass();

  // Get the Error
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    // std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
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
  else if(e.type == SDL_MOUSEBUTTONDOWN)
  {
    if(e.button.button == SDL_BUTTON_LEFT)
    {
      printf("Test\n");
    }
    if(e.button.button == SDL_BUTTON_MIDDLE)
    {
      window->ChangeMode();
    }
  }
  else if(e.type == SDL_MOUSEBUTTONUP)
  {
    if(e.button.button == SDL_BUTTON_LEFT)
    {
      printf("Test\n");
    }
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
      camera->strafe(speed * dt);
    }
    // move back
    if (e.key.keysym.sym == SDLK_s)
    {
      camera->translate(-speed * dt);
    }

    // move right
    if (e.key.keysym.sym == SDLK_d)
    {
      camera->strafe(-speed * dt);
    }

    // move forward
    if (e.key.keysym.sym == SDLK_w)
    {
      camera->translate(speed * dt);
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
      printf("The Camera Position: (%.2f, %.2f, %.2f)\n", 
              camera->getPos().x, camera->getPos().y, camera->getPos().z);
      printf("The Camera Focus: (%.2f, %.2f, %.2f)\n", 
              camera->getFocus().x, camera->getFocus().y, camera->getFocus().z);
      printf("The placement data: (%.2f, %.2f, %.2f)\n", x, y, z);
      //printf("Random %.2f\n", RandomFloat());
    }
    if (e.key.keysym.sym == SDLK_y)
    {
      house->Move(1.0);
    }
    if (e.key.keysym.sym == SDLK_u)
    {
      house->Move(-1.0);
    }
    if (e.key.keysym.sym == SDLK_n)
    {
      if(terrain->ToggleNight())
      {
        DirLight.Color = COLOR_NIGHT;
        DirLight.AmbientIntensity = 0.1f;
        DirLight.DiffuseIntensity = 0.9f;

        // Skybox
        delete skyBox;
        skyBox = new SkyBox();
        if (!skyBox->Init("../content/", "lostatseanight_right.jpg", "lostatseanight_left.jpg", "lostatseanight_top.jpg", 
                          "lostatseanight_top.jpg", "lostatseanight_front.jpg", "lostatseanight_back.jpg")) 
        {
          printf("Skybox Failed to init.\n");
          return false;
        }

        fireworks = new ParticleSystem(); 
        if(!fireworks->InitParticleSystem(glm::vec3(-114.92, 132.96, 225.02)))
        {
          printf("Particle System Failed to init.\n");
          delete fireworks;
          fireworks = NULL;
          return false;
        }

        for (unsigned int i = 0 ; i < numPointLights; i++)
        {
          pointLight[i].on = true;
        }
      }
      else
      {
        DirLight.AmbientIntensity = 0.1f;
        DirLight.Color = COLOR_WHITE;
        DirLight.DiffuseIntensity = 0.5f;

        // Skybox
        delete fireworks;
        fireworks = NULL;
        delete skyBox;
        skyBox = new SkyBox();
        if (!skyBox->Init("../content/", "bluesky_right.jpg", "bluesky_left.jpg", "bluesky_top.jpg", 
                             "bluesky_top.jpg", "bluesky_front.jpg", "bluesky_back.jpg")) 
        {
          printf("Skybox Failed to init.\n");
          return false;
        }

        for (unsigned int i = 0 ; i < numPointLights; i++)
        {
          pointLight[i].on = false;
        }
      }

    }
    if (e.key.keysym.sym == SDLK_t)
    {
      selectMoveItem++;
      if(selectMoveItem > 2)
      {
        selectMoveItem = 0;
      }
    }
    if (e.key.keysym.sym == SDLK_i)
    {
      if(selectMoveItem == 0)
      {
        x -= 0.1;
      }
      else if(selectMoveItem == 1)
      {
        y -= 0.1;
      }
      else
      {
        z -= 0.1;
      }
    }
    if (e.key.keysym.sym == SDLK_o)
    {
      if(selectMoveItem == 0)
      {
        x += 0.1;
      }
      else if(selectMoveItem == 1)
      {
        y += 0.1;
      }
      else
      {
        z += 0.1;
      }
    }
    if (e.key.keysym.sym == SDLK_l)
    {
      if(speed == 0.1f)
      {
        speed = 0.01f;
      }
      else
      {
        speed = 0.1f;
      }
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
  if(e.type == SDL_MOUSEMOTION)
  {
    if(window->GetMode())
    {
      camera->rotateX(-e.motion.xrel * 0.1f * dt);
      camera->rotateY(-e.motion.yrel * 0.1f * dt);    
    }
  }

  // Return Success
  return true;
}
