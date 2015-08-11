// Includes for OpenGL related
#include <SDL2/SDL.h>

#include <iostream>
using namespace std;

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  #include <GL/glu.h>
#endif

// C++ Related Includes
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <chrono>
#include <gbuffer.h>
#include <null_technique.h>
#include <math.h>
#include "ogldev_engine_common.h"
#include "ogldev_camera.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ds_geom_pass_tech.h"
#include "ds_point_light_pass_tech.h"
#include "ds_dir_light_pass_tech.h"
#include "null_technique.h"
#include "ogldev_basic_mesh.h"
#include "gbuffer.h"
#include "lights_common.h"
#include "particle_system.h"
#include "ogldev_basic_lighting.h"
#include "mesh.h"


#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

// Render Screen Width & height
int width = 640, height = 480;

// OpenGL necessary functions
bool initilize();
void InitLights();
float CalcPointLightBSphere(const PointLight& Light);
bool update();
void render();
bool keyboard(float dt);
void close();
std::string ErrorString(GLenum error);
void DSGeometryPass();
void DSStencilPass(unsigned int PointLightIndex);
void DSPointLightPass(unsigned int PointLightIndex);
void DSDirectionalLightPass();
void DSFinalPass();

// SDL data
SDL_Window* gWindow = NULL;
SDL_GLContext gContext;

//Time function
float getdt();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

// OpenGL Buffers
GLuint VBO;
GLint VaoId;
GLint loc_mvpmat;

// Toggles
bool SCALE = false;
int MOVE_INDEX = 0;

// Deferred Shading Stuff
DSGeomPassTech m_DSGeomPassTech;
DSPointLightPassTech m_DSPointLightPassTech;
DSDirLightPassTech m_DSDirLightPassTech;
NullTechnique m_nullTech;
Camera* m_pGameCamera;
float m_scale;
SpotLight m_spotLight;
DirectionalLight m_dirLight;
PointLight m_pointLight[3];
BasicMesh m_bsphere;
BasicMesh m_quad;
PersProjInfo m_persProjInfo;
GBuffer m_gbuffer;

long long m_currentTimeMillis;
BasicLightingTechnique* m_pLightingTechnique;
Mesh* m_pGround;    
Texture* m_pTexture;
Texture* m_pNormalMap;
ParticleSystem m_particleSystem;

// Main Program
int main(int argc, char **argv)
{
  // Init the screen
  if(!initilize())
  {
    printf("Failed to startup\n");
    return 1;
  }

  bool run = true;
  // Startup
  while(run)
  {
    run = update();
    render();
  }

  // End
  close();
  return 0;
}

bool initilize()
{
  // Start SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL failed to initialize: %s\n", SDL_GetError());
    return false;
  }

  // Start OpenGL
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

  // Create window
  gWindow = SDL_CreateWindow("Tutorial Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
  if(gWindow == NULL)
  {
    printf("Widow failed to create: %s\n", SDL_GetError());
    return false;
  }

  // Create context
  gContext = SDL_GL_CreateContext(gWindow);
  if(gContext == NULL)
  {
    printf("OpenGL context not created: %s\n", SDL_GetError());
    return false;
  }

  // Use VSync
  if(SDL_GL_SetSwapInterval(1) < 0)
  {
    printf("Unable to use VSync: %s\n", SDL_GetError());
  }

  // Start Text Input
  SDL_StartTextInput();

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init values
  m_pLightingTechnique = NULL;
  m_pGameCamera = NULL;
  m_pTexture = NULL;
  m_pGround = NULL;
  m_pNormalMap = NULL;
  m_scale = 0.0f;
  m_persProjInfo.FOV = 60.0f;
  m_persProjInfo.Height = WINDOW_HEIGHT;
  m_persProjInfo.Width = WINDOW_WIDTH;
  m_persProjInfo.zNear = 1.0f;
  m_persProjInfo.zFar = 100.0f;  
  InitLights();
  m_currentTimeMillis = GetCurrentTimeMillis();

  Vector3f Pos(0.0f, 0.4f, -0.5f);
  Vector3f Target(0.0f, 0.2f, 1.0f);
  Vector3f Up(0.0, 1.0f, 0.0f);

  m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

/*
  // Setup the gbuffer
  if(!m_gbuffer.Init(WINDOW_WIDTH, WINDOW_HEIGHT))
  {
    std::cout<<"GBuffer Init failed.\n";
    return false;
  }

  if(!m_DSGeomPassTech.Init())
  {
    std::cout<<"GPass Init failed.\n";
    return false;
  }

  m_DSGeomPassTech.Enable();
  m_DSGeomPassTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
  

  if (!m_DSPointLightPassTech.Init()) 
  {
    printf("Error initializing DSPointLightPassTech\n");
    return false;
  }

  m_DSPointLightPassTech.Enable();

  m_DSPointLightPassTech.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  m_DSPointLightPassTech.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  m_DSPointLightPassTech.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  m_DSPointLightPassTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!m_DSDirLightPassTech.Init())
  {
    printf("Error initializing DSDirLightPassTech\n");
    return false;
  }

  m_DSDirLightPassTech.Enable();

  m_DSDirLightPassTech.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  m_DSDirLightPassTech.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  m_DSDirLightPassTech.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);
  m_DSDirLightPassTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  Matrix4f WVP;
  WVP.InitIdentity();
  m_DSDirLightPassTech.SetWVP(WVP);

  if(!m_nullTech.Init())
  {
    printf("Null Tech Failed\n");
    return false;
  }

  if(!m_quad.LoadMesh("../Content/quad.obj"))
  {
    printf("There was an error initiating the: quad\n");
    return false;
  }            

  if(!m_bsphere.LoadMesh("../Content/sphere.obj"))
  {
    printf("There was an error initiating the: sphere\n");
    return false;
  }
*/
  m_pLightingTechnique = new BasicLightingTechnique();

  if (!m_pLightingTechnique->Init())
  {
    printf("Error initializing the basic lighting technique\n");
    return false;
  }

  m_pLightingTechnique->Enable();
  m_pLightingTechnique->SetDirectionalLight(m_dirLight);
  m_pLightingTechnique->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

  m_pGround = new Mesh();
  
  if (!m_pGround->LoadMesh("../Content/quad3.obj")) {
      return false;
  }
                 
  m_pTexture = new Texture(GL_TEXTURE_2D, "../Content/bricks.jpg");
  
  if (!m_pTexture->Load()) {
      return false;
  }
  
  m_pTexture->Bind(COLOR_TEXTURE_UNIT);

  m_pNormalMap = new Texture(GL_TEXTURE_2D, "../Content/normal_map.jpg");
  
  if (!m_pNormalMap->Load()) {
      return false;
  }
  
  Vector3f ParticleSystemPos = Vector3f(0.0f, 0.0f, 1.0f);
                  
  if(!m_particleSystem.InitParticleSystem(ParticleSystemPos))
  {
    printf("Particle System Failed to init.\n");
    return false;
  }

  return true;
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

void InitLights()
{
  m_spotLight.AmbientIntensity = 0.0f;
  m_spotLight.DiffuseIntensity = 0.9f;
  m_spotLight.Color = COLOR_WHITE;
  m_spotLight.Attenuation.Linear = 0.01f;
  m_spotLight.Position  = Vector3f(-20.0, 20.0, 5.0f);
  m_spotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
  m_spotLight.Cutoff =  20.0f;

  m_dirLight.AmbientIntensity = 0.2f;
  m_dirLight.Color = COLOR_WHITE;
  m_dirLight.DiffuseIntensity = 0.8f;
  m_dirLight.Direction = Vector3f(1.0f, 0.0f, 0.0f);

  m_pointLight[0].DiffuseIntensity = 0.2f;
  m_pointLight[0].Color = COLOR_GREEN;
  m_pointLight[0].Position = Vector3f(0.0f, 1.5f, 5.0f);
  m_pointLight[0].Attenuation.Constant = 0.0f;
  m_pointLight[0].Attenuation.Linear = 0.0f;
  m_pointLight[0].Attenuation.Exp = 0.3f;

  m_pointLight[1].DiffuseIntensity = 0.2f;
  m_pointLight[1].Color = COLOR_RED;
  m_pointLight[1].Position = Vector3f(2.0f, 0.0f, 5.0f);
  m_pointLight[1].Attenuation.Constant = 0.0f;
  m_pointLight[1].Attenuation.Linear = 0.0f;
  m_pointLight[1].Attenuation.Exp = 0.3f;

  m_pointLight[2].DiffuseIntensity = 0.2f;
  m_pointLight[2].Color = COLOR_BLUE;
  m_pointLight[2].Position = Vector3f(0.0f, 0.0f, 3.0f);
  m_pointLight[2].Attenuation.Constant = 0.0f;
  m_pointLight[2].Attenuation.Linear = 0.0f;        
  m_pointLight[2].Attenuation.Exp = 0.3f;
}

// The calculation solves a quadratic equation (see http://en.wikipedia.org/wiki/Quadratic_equation)
float CalcPointLightBSphere(const PointLight& Light)
{
  float MaxChannel = fmax(fmax(Light.Color.x, Light.Color.y), Light.Color.z);
  
  float ret = (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel * Light.DiffuseIntensity))) 
              /
              2 * Light.Attenuation.Exp;
  
  return ret;
}   

bool update()
{
  if(!keyboard(1.0f))
  {
    return false;
  }
  
  // All good
  return true;
}

void DSGeometryPass()
{
  m_DSGeomPassTech.Enable();

  m_gbuffer.BindForGeomPass();

  // Only the geometry pass updates the depth buffer
  glDepthMask(GL_TRUE);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  Pipeline p;
  p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
  p.SetPerspectiveProj(m_persProjInfo);        
  p.Rotate(0.0f, m_scale, 0.0f);


  // When we get here the depth buffer is already populated and the stencil pass
  // depends on it, but it does not write to it.
  glDepthMask(GL_FALSE);    
}

void DSStencilPass(unsigned int PointLightIndex)
{
  m_nullTech.Enable();

  // Disable color/depth write and enable stencil
  m_gbuffer.BindForStencilPass();
  glEnable(GL_DEPTH_TEST);

  glDisable(GL_CULL_FACE);

  glClear(GL_STENCIL_BUFFER_BIT);

  // We need the stencil test to be enabled but we want it
  // to succeed always. Only the depth test matters.
  glStencilFunc(GL_ALWAYS, 0, 0);

  glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
  glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

  Pipeline p;
  p.WorldPos(m_pointLight[PointLightIndex].Position);
  float BBoxScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);
  p.Scale(BBoxScale, BBoxScale, BBoxScale);   
  p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
  p.SetPerspectiveProj(m_persProjInfo);

  m_nullTech.SetWVP(p.GetWVPTrans());
  m_bsphere.Render();  
}

    
void DSPointLightPass(unsigned int PointLightIndex)
{
  m_gbuffer.BindForLightPass();

  m_DSPointLightPassTech.Enable();
  m_DSPointLightPassTech.SetEyeWorldPos(m_pGameCamera->GetPos());        

  glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  Pipeline p;
  p.WorldPos(m_pointLight[PointLightIndex].Position);
  float BBoxScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);        
  p.Scale(BBoxScale, BBoxScale, BBoxScale);   
  p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
  p.SetPerspectiveProj(m_persProjInfo);               
  m_DSPointLightPassTech.SetWVP(p.GetWVPTrans());
  m_DSPointLightPassTech.SetPointLight(m_pointLight[PointLightIndex]);
  m_bsphere.Render(); 
  glCullFace(GL_BACK);

  glDisable(GL_BLEND);
}
  

void DSDirectionalLightPass()
{
  m_gbuffer.BindForLightPass();

  m_DSDirLightPassTech.Enable();
  m_DSDirLightPassTech.SetEyeWorldPos(m_pGameCamera->GetPos());

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);

  m_quad.Render();  

  glDisable(GL_BLEND);
}


void DSFinalPass()
{
  m_gbuffer.BindForFinalPass();
  glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 
                    0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void render()
{
/*
  m_scale += 0.05f;

  m_pGameCamera->OnRender();

  m_gbuffer.StartFrame();

  DSGeometryPass();

  // We need stencil to be enabled in the stencil pass to get the stencil buffer
  // updated and we also need it in the light pass because we render the light
  // only if the stencil passes.
  glEnable(GL_STENCIL_TEST);

  for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLight); i++)
  {
    DSStencilPass(i);
    DSPointLightPass(i);
  }

  // The directional light does not need a stencil test because its volume
  // is unlimited and the final pass simply copies the texture.
  glDisable(GL_STENCIL_TEST);

  DSDirectionalLightPass();

  DSFinalPass();
*/

  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  m_pGameCamera->OnRender();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_pLightingTechnique->Enable();

  m_pTexture->Bind(COLOR_TEXTURE_UNIT);       
  m_pNormalMap->Bind(NORMAL_TEXTURE_UNIT);

  Pipeline p;
  p.Scale(20.0f, 20.0f, 1.0f);
  p.Rotate(90.0f, 0.0, 0.0f);
  p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
  p.SetPerspectiveProj(m_persProjInfo);

  m_pLightingTechnique->SetWVP(p.GetWVPTrans());
  m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());

  m_pGround->Render();

  m_particleSystem.Render(DeltaTimeMillis, p.GetVPTrans(), m_pGameCamera->GetPos());

  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }

  SDL_GL_SwapWindow(gWindow);
}

bool keyboard(float dt)
{
  SDL_Event e;
  while(SDL_PollEvent(&e) != 0)
  {
    if(e.type == SDL_QUIT)
    {
      return false;
    }
    else if(e.type == SDL_KEYDOWN)
    {
      // Escape Key
      if(e.key.keysym.sym == SDLK_ESCAPE)
      {
        return false;
      }
/*
      // Print Camera
      if(e.key.keysym.sym == SDLK_p)
      {
        camera.print();
        printf("Location: %f, %f, %f\n", m_pointLight[MOVE_INDEX].Position.x, m_pointLight[MOVE_INDEX].Position.y, m_pointLight[MOVE_INDEX].Position.z);
      }

      // move up
      if (e.key.keysym.sym == SDLK_w)
      {
        camera.moveVerticle(1.0);
      }

      // move down
      if (e.key.keysym.sym == SDLK_s)
      {
        camera.moveVerticle(-1.0);
      }

      // move left
      if (e.key.keysym.sym == SDLK_a)
      {
        camera.moveHorizontal(-1.0);
      }

      // move right
      if (e.key.keysym.sym == SDLK_d)
      {
        camera.moveHorizontal(1.0);
      }

      // move forward
      if (e.key.keysym.sym == SDLK_q)
      {
        camera.moveLateral(-1.0);
      }

      // move backward
      if (e.key.keysym.sym == SDLK_e)
      {
        camera.moveLateral(1.0);
      }

      // focus up
      if (e.key.keysym.sym == SDLK_t)
      {
        camera.focusVerticle(1.0);
      }

      // focus down
      if (e.key.keysym.sym == SDLK_g)
      {
        camera.focusVerticle(-1.0);
      }

      // focus left
      if (e.key.keysym.sym == SDLK_f)
      {
        camera.focusHorizontal(-1.0);
      }

      // focus right
      if (e.key.keysym.sym == SDLK_h)
      {
        camera.focusHorizontal(1.0);
      }

      // focus forward
      if (e.key.keysym.sym == SDLK_r)
      {
        camera.focusLateral(-1.0);
      }

      // focus backward
      if (e.key.keysym.sym == SDLK_y)
      {
        camera.focusLateral(1.0);
      }

      if(e.key.keysym.sym == SDLK_m)
      {
        m_pointLight[MOVE_INDEX].Position = glm::vec3(m_pointLight[MOVE_INDEX].Position.x+2, m_pointLight[MOVE_INDEX].Position.y, m_pointLight[MOVE_INDEX].Position.z);
      }
      if(e.key.keysym.sym == SDLK_n)
      {
        m_pointLight[MOVE_INDEX].Position = glm::vec3(m_pointLight[MOVE_INDEX].Position.x-2, m_pointLight[MOVE_INDEX].Position.y, m_pointLight[MOVE_INDEX].Position.z);
      }
      if(e.key.keysym.sym == SDLK_b)
      {
        m_pointLight[MOVE_INDEX].Position = glm::vec3(m_pointLight[MOVE_INDEX].Position.x, m_pointLight[MOVE_INDEX].Position.y+2, m_pointLight[MOVE_INDEX].Position.z);
      }
      if(e.key.keysym.sym == SDLK_v)
      {
        m_pointLight[MOVE_INDEX].Position = glm::vec3(m_pointLight[MOVE_INDEX].Position.x, m_pointLight[MOVE_INDEX].Position.y-2, m_pointLight[MOVE_INDEX].Position.z);
      }
      if(e.key.keysym.sym == SDLK_c)
      {
        m_pointLight[MOVE_INDEX].Position = glm::vec3(m_pointLight[MOVE_INDEX].Position.x, m_pointLight[MOVE_INDEX].Position.y, m_pointLight[MOVE_INDEX].Position.z+2);
      }
      if(e.key.keysym.sym == SDLK_x)
      {
        m_pointLight[MOVE_INDEX].Position = glm::vec3(m_pointLight[MOVE_INDEX].Position.x, m_pointLight[MOVE_INDEX].Position.y, m_pointLight[MOVE_INDEX].Position.z-2);
      }
*/

      if (e.key.keysym.sym == SDLK_MINUS)
      {
        m_dirLight.DiffuseIntensity += 0.1f;
        m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);
      }
      if (e.key.keysym.sym == SDLK_EQUALS)
      {
        m_dirLight.DiffuseIntensity -= 0.1f;
        m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);
      }

      if (e.key.keysym.sym == SDLK_z)
      {
        SCALE = !SCALE;
      }
      if (e.key.keysym.sym == SDLK_o)
      {
        MOVE_INDEX++;
        if(MOVE_INDEX > 2)
        {
          MOVE_INDEX = 0;
        }
      }
    }
  }

  // No end to program
  return true;
}

void close()
{
  SDL_StopTextInput();
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  SDL_Quit();
}

/*
unsigned int getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}
*/

float getdt()
{
  float ret;
  t2 = std::chrono::high_resolution_clock::now();
  ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
  t1 = std::chrono::high_resolution_clock::now();
  return ret;
}
