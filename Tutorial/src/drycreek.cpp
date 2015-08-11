// Includes for OpenGL related
#include <SDL2/SDL.h>
#include <opengl_data.h>

// C++ Related Includes
#include <iostream>
#include <chrono>
#include <shader.h>
#include <camera.h>
#include <object.h>
#include <geom_pass.h>
#include <point_pass.h>
#include <directional_pass.h>
#include <gbuffer.h>
#include <skybox_tech.h>
#include <skybox.h>
#include <particle_system.h>
#include <null_technique.h>
#include <terrain.h>

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
void geometryPass();
void beginLightPass();
void pointLightPass(int i);
void DSStencilPass(unsigned int PointLightIndex);
void directLightPass();
void DSFinalPass();

// SDL data
SDL_Window* gWindow = NULL;
SDL_GLContext gContext;
Camera camera;

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

// Object
Object box[5];
Object platform[5];

// Deferred Shading Stuff
GeomPass g_pass;
GBuffer g_buffer;
DSPointLightPassTech m_DSPointLightPassTech;
DSDirLightPassTech m_DSDirLightPassTech;
SpotLight m_spotLight;
DirectionalLight m_dirLight;
PointLight m_pointLight[3];
Object m_bsphere;
Object m_quad;
SkyBox* m_pSkyBox;
ParticleSystem m_particleSystem;
NullTechnique m_nullTech;
long long m_currentTimeMillis;
terrain Terrain;


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

  // Init the lights
  InitLights();

  // Create window
  gWindow = SDL_CreateWindow("Tutorial Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
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

  // Init glew if not a mac
  #if !defined(__APPLE__) && !defined(MACOSX)
    cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();
    //Check for error
    if (status != GLEW_OK)
    {
      //std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // Start Text Input
  SDL_StartTextInput();

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Setup the triangle
  //glGenBuffers(1, &VBO);
  //glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);

  // Setup the gbuffer
  if(!g_buffer.Init(width, height))
  {
    std::cout<<"GBuffer Init failed.\n";
    return false;
  }
  
  if(!g_pass.Init())
  {
    std::cout<<"GPass Init failed.\n";
    return false;
  }
  g_pass.Start();
  g_pass.SetTexture(0);
  

  if (!m_DSPointLightPassTech.Init()) 
  {
    printf("Error initializing DSPointLightPassTech\n");
    return false;
  }

  m_DSPointLightPassTech.Start();

  m_DSPointLightPassTech.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  m_DSPointLightPassTech.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  m_DSPointLightPassTech.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  m_DSPointLightPassTech.SetScreenSize(width, height);

  if (!m_DSDirLightPassTech.Init())
  {
    printf("Error initializing DSDirLightPassTech\n");
    return false;
  }

  m_DSDirLightPassTech.Start();

  m_DSDirLightPassTech.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  m_DSDirLightPassTech.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  m_DSDirLightPassTech.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);
  m_DSDirLightPassTech.SetScreenSize(width, height);
  m_DSDirLightPassTech.SetWVP(glm::mat4(1.0f));

  if(!m_nullTech.Init())
  {
    printf("Null Tech Failed\n");
    return false;
  }

  Terrain.SetFile("../data/drycreek.tif");
  Terrain.setup();

  for(int i = 0; i < 5; i++)
  {
    if(!box[i].LoadMesh("../data/wood_box.obj"))
    {
      printf("There was an error loading one box\n");
      return false;
    }
  }

  for(int i = 0; i < 5; i++)
  {
    if(!platform[i].LoadMesh("../data/earth.obj"))
    {
      printf("There was an error loading one earth\n");
      return false;
    }
    platform[i].toggle();
  }
  
  box[0].setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
  box[1].setPosition(glm::vec3(6.0f, 1.0f, 10.0f));
  box[2].setPosition(glm::vec3(-5.0f, -1.0f, 12.0f));
  box[3].setPosition(glm::vec3(4.0f, 4.0f, 15.0f));
  box[4].setPosition(glm::vec3(-4.0f, 2.0f, 20.0f));
  platform[0].setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
  platform[1].setPosition(glm::vec3(6.0f, 1.0f, 10.0f));
  platform[2].setPosition(glm::vec3(-5.0f, -1.0f, 12.0f));
  platform[3].setPosition(glm::vec3(4.0f, 4.0f, 15.0f));
  platform[4].setPosition(glm::vec3(-4.0f, 2.0f, 20.0f));
  

  if(!m_quad.LoadMesh("../data/sphere.obj"))
  {
    printf("Quad Failed to load.\n");
    return false;
  }

  if(!m_bsphere.LoadMesh("../data/sphere.obj"))
  {
    printf("Sphere failed to load.\n");
    return false;
  }


  // Skybox
  m_pSkyBox = new SkyBox();

  if (!m_pSkyBox->Init(".", "../data/sp3right.jpg", "../data/sp3left.jpg", "../data/sp3top.jpg", "../data/sp3bot.jpg", "../data/sp3front.jpg", "../data/sp3back.jpg")) 
  {
    return false;
  }

  if(!m_particleSystem.InitParticleSystem(glm::vec3(0.0f, 0.0f, 5.0f)))
  {
    printf("Particle System Failed\n");
    return false;
  }

  // Camera
  camera.setThresh(20,10);
  //camera.viewUpdate();

  return true;
}

void InitLights()
{
  m_spotLight.AmbientIntensity = 0.0f;
  m_spotLight.DiffuseIntensity = 0.9f;
  m_spotLight.Color = COLOR_WHITE;
  m_spotLight.Attenuation.Linear = 0.01f;
  m_spotLight.Position  = glm::vec3(-20.0, 20.0, 5.0f);
  m_spotLight.Direction = glm::vec3(1.0f, -1.0f, 0.0f);
  m_spotLight.Cutoff =  20.0f;

  m_dirLight.AmbientIntensity = 0.1f;
  m_dirLight.Color = COLOR_WHITE;
  m_dirLight.DiffuseIntensity = 0.5f;
  m_dirLight.Direction = glm::vec3(1.0f, 0.0f, 0.0f);

  m_pointLight[0].DiffuseIntensity = 10.2f;
  m_pointLight[0].Color = COLOR_GREEN;
  m_pointLight[0].Position = glm::vec3(0.0f, 1.5f, 15.0f);
  m_pointLight[0].Attenuation.Constant = 0.0f;
  m_pointLight[0].Attenuation.Linear = 0.0f;
  m_pointLight[0].Attenuation.Exp = 0.3f;

  m_pointLight[1].DiffuseIntensity = 10.2f;
  m_pointLight[1].Color = COLOR_RED;
  m_pointLight[1].Position = glm::vec3(2.0f, 2.0f, 11.0f);
  m_pointLight[1].Attenuation.Constant = 0.0f;
  m_pointLight[1].Attenuation.Linear = 0.0f;
  m_pointLight[1].Attenuation.Exp = 0.3f;

  m_pointLight[2].DiffuseIntensity = 10.2f;
  m_pointLight[2].Color = COLOR_BLUE;
  m_pointLight[2].Position = glm::vec3(0.0f, 0.0f, 9.0f);
  m_pointLight[2].Attenuation.Constant = 0.0f;
  m_pointLight[2].Attenuation.Linear = 0.0f;        
  m_pointLight[2].Attenuation.Exp = 0.3f;
}

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
  static float angle = 0.0;
  float dt = getdt();
  angle += dt * M_PI/4;

  if(!keyboard(dt))
  {
    return false;
  }
/*
  for(int i = 0; i < 5; i++)
  {
    box[i].model = glm::translate(glm::mat4(1.0f), box[i].getPosition()) * glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));
    platform[i].model = glm::translate(glm::mat4(1.0f), platform[i].getPosition()) * glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));
    //box[i].model = glm::scale(box[i].model, glm::vec3(2,2,2));
  }
  */
  // All good
  return true;
}

void geometryPass()
{

  g_pass.Start();
  //g_buffer.BindForWriting();
  g_buffer.BindForGeomPass();
  
  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);


  for(int i = 1; i < (sizeof(box)/sizeof(box[0])); i++)
  {
    if(box[i].ison())
    {
      glm::mat4 mvp = camera.getProjection() * camera.getView() * box[i].model;
      g_pass.SetMVP(mvp);
      g_pass.SetModel(box[i].model);
      box[i].Render();
    }
    else
    {
      glm::mat4 mvp = camera.getProjection() * camera.getView() * platform[i].model;
      g_pass.SetMVP(mvp);
      g_pass.SetModel(platform[i].model);
      platform[i].Render(); 
    }
  }


  Terrain.render(camera.getView(), camera.getProjection());

  // When we get here the depth buffer is already populated and the stencil pass
  // depends on it, but it does not write to it.
  glDepthMask(GL_FALSE);
}

void beginLightPass()
{
  // Begin Passes
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);
  g_buffer.BindForReading();
  glClear(GL_COLOR_BUFFER_BIT);
}

void pointLightPass(int i)
{
  g_buffer.BindForLightPass();

  // Point Light Pass
  m_DSPointLightPassTech.Start();
  m_DSPointLightPassTech.SetEyeWorldPos(glm::vec3(camera.x(), camera.y(), camera.z()));

  glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  glm::vec3 worldPos = m_pointLight[i].Position;
  float BSphereScale = CalcPointLightBSphere(m_pointLight[i]);
  m_bsphere.model = glm::translate(glm::mat4(1.0f), worldPos);

  if(SCALE)
  {
    m_bsphere.model = glm::scale(m_bsphere.model, glm::vec3(BSphereScale, BSphereScale, BSphereScale));
  }
  
  glm::mat4 mvp = camera.getProjection() * camera.getView() * m_bsphere.model;
  m_DSPointLightPassTech.SetWVP(mvp);
  m_DSPointLightPassTech.SetPointLight(m_pointLight[i]);            
  m_bsphere.Render();

  glCullFace(GL_BACK);
  glDisable(GL_BLEND);
}

void DSStencilPass(unsigned int PointLightIndex)
{
  m_nullTech.Start();

  // Disable color/depth write and enable stencil
  g_buffer.BindForStencilPass();

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glClear(GL_STENCIL_BUFFER_BIT);

  // We need the stencil test to be enabled but we want it
  // to succeed always. Only the depth test matters.
  glStencilFunc(GL_ALWAYS, 0, 0);

  glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
  glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

  glm::vec3 worldPos = m_pointLight[PointLightIndex].Position;
  float BBoxScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);

  m_bsphere.model = glm::translate(glm::mat4(1.0f), worldPos);

  if(SCALE)
  {
    m_bsphere.model = glm::scale(m_bsphere.model, glm::vec3(BBoxScale, BBoxScale, BBoxScale));
  }

  glm::mat4 mvp = camera.getProjection() * camera.getView() * m_bsphere.model;
  m_nullTech.SetWVP(mvp);
  m_bsphere.Render();

}


void directLightPass()
{
  g_buffer.BindForLightPass();

  // Directional Light Pass
  m_DSDirLightPassTech.Start();
  m_DSDirLightPassTech.SetEyeWorldPos(glm::vec3(camera.x(), camera.y(), camera.z()));
  m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);

  m_quad.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 10.0, 0.0));
  m_quad.model = glm::scale(m_quad.model, glm::vec3(2,2,2));
  glm::mat4 loc = glm::mat4(1.0);
  m_DSDirLightPassTech.SetWVP(loc);
  m_quad.Render();
  
  glDisable(GL_BLEND);
}

void DSFinalPass()
{
  g_buffer.BindForFinalPass();
  glBlitFramebuffer(0, 0, width, height, 
                    0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void render()
{ 
  //m_particleSystem.Render(getDT(), glm::translate(glm::mat4(1.0f), glm::vec3(0, 10, 0)), 
  //                        glm::vec3(camera.x(), camera.y(), camera.z()));
  //m_pSkyBox->Render(camera.getProjection(), camera.getView(), 
  //                  glm::translate(glm::mat4(1.0f), glm::vec3(camera.x(), camera.y(), camera.z())));

  //m_particleSystem.Render(getDT(), glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0)), 
  //                      glm::vec3(camera.x(), camera.y(), camera.z()));

  g_buffer.StartFrame();



  //m_pSkyBox->Render(camera.getProjection(), camera.getView(), 
  //                  glm::translate(glm::mat4(1.0f), glm::vec3(camera.x(), camera.y(), camera.z())));

  geometryPass();
  

  glEnable(GL_STENCIL_TEST);
  for (unsigned int i = 0 ; i < (sizeof(m_pointLight)/sizeof(m_pointLight[0])); i++)
  {
    DSStencilPass(i);
    pointLightPass(i);
  }
  glDisable(GL_STENCIL_TEST);
  


  //beginLightPass();
  //pointLightPass();
  directLightPass();

  //long long TimeNowMillis = GetCurrentTimeMillis();
  //assert(TimeNowMillis >= m_currentTimeMillis);
  //unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  //m_currentTimeMillis = TimeNowMillis;

  //m_particleSystem.Render(DeltaTimeMillis, glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0)), 
  //                      glm::vec3(camera.x(), camera.y(), camera.z()));

  DSFinalPass();

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

      if (e.key.keysym.sym == SDLK_i)
      {
        for(int i = 0; i < (sizeof(box)/sizeof(box[0])); i++)
        {
          box[i].toggle();
        }
        for(int i = 0; i < (sizeof(platform)/sizeof(platform[0])); i++)
        {
          platform[i].toggle();
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
