// Includes for OpenGL related
#include <SDL2/SDL.h>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  #include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

// Render Screen Width & height
int width = 640, height = 480;

// OpenGL necessary functions
bool initilize();
void InitLights();
float CalcPointLightBSphere(const PointLight& Light);
bool update();
string ErrorString(GLenum error);
void render();
bool keyboard(float dt);
void close();
void beginLightPass();
void pointLightPass();
void directLightPass();

// SDL data
SDL_Window* gWindow = NULL;
SDL_GLContext gContext;
Camera camera;

//Time function
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

// OpenGL Buffers
GLuint VBO;
GLint VaoId;
GLint loc_mvpmat;

// Temp model
glm::mat4 model;

// Object
Object box[5];
Object platform;

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
  

  for(int i = 0; i < 5; i++)
  {
    if(!box[i].LoadMesh("../data/wood_box.obj"))
    {
      printf("There was an error loading one box\n");
      return false;
    }
  }
  //float angle = 0.3 * M_PI/2;
  
  box[0].setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
  box[1].setPosition(glm::vec3(6.0f, 1.0f, 10.0f));
  box[2].setPosition(glm::vec3(-5.0f, -1.0f, 12.0f));
  box[3].setPosition(glm::vec3(4.0f, 4.0f, 15.0f));
  box[4].setPosition(glm::vec3(-4.0f, 2.0f, 20.0f));
  /*
  box[0].model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f));
  box[1].model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 1.0f, 10.0f));
  box[2].model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -1.0f, 12.0f));
  box[3].model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 15.0f));
  box[4].model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 2.0f, 20.0f));
  */
  if(!m_quad.LoadMesh("../data/quad.obj"))
  {
    printf("Quad Failed to load.\n");
    return false;
  }

  if(!m_bsphere.LoadMesh("../data/sphere.obj"))
  {
    printf("Sphere failed to load.\n");
    return false;
  }
  //platform.LoadMesh("../data/earth.obj");
  
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

  m_dirLight.AmbientIntensity = 0.5f;
  m_dirLight.Color = COLOR_CYAN;
  m_dirLight.DiffuseIntensity = 0.5f;
  m_dirLight.Direction = glm::vec3(0.0f, -1.0f, 0.0f);

  m_pointLight[0].DiffuseIntensity = 10.2f;
  m_pointLight[0].Color = COLOR_GREEN;
  m_pointLight[0].Position = glm::vec3(0.0f, 1.5f, 5.0f);
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
  m_pointLight[2].Position = glm::vec3(0.0f, 0.0f, 3.0f);
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
  float dt = getDT();
  angle += dt * M_PI/4;

  if(!keyboard(dt))
  {
    return false;
  }

  for(int i = 0; i < 5; i++)
  {
    box[i].model = glm::translate(glm::mat4(1.0f), box[i].getPosition()) * glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));
  }
  
  //platform.model = glm::rotate(glm::mat4(1.0f), (angle), glm::vec3(0.0, 1.0, 0.0));

  // All good
  return true;
}

string ErrorString(GLenum error)
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

void beginLightPass()
{
  // Begin Passes
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE);
  g_buffer.BindForReading();
  glClear(GL_COLOR_BUFFER_BIT);
}

void pointLightPass()
{
  // Point Light Pass
  m_DSPointLightPassTech.Start();
  m_DSPointLightPassTech.SetEyeWorldPos(glm::vec3(camera.x(), camera.y(), camera.z()));        

  for (unsigned int i = 0 ; i < (sizeof(m_pointLight)/sizeof(m_pointLight[0])); i++) 
  {
    m_DSPointLightPassTech.SetPointLight(m_pointLight[i]);            
    //p.WorldPos(m_pointLight[i].Position);
    glm::vec3 worldPos = m_pointLight[i].Position;
    float BSphereScale = CalcPointLightBSphere(m_pointLight[i]);
    m_bsphere.model = glm::translate(glm::mat4(1.0f), worldPos) ;//* glm::scale(glm::mat4(1.0f), glm::vec3(BSphereScale, BSphereScale, BSphereScale));
    //p.Scale(BSphereScale, BSphereScale, BSphereScale);  
    //m_DSPointLightPassTech.SetWVP(p.GetWVPTrans());
    //glm::mat4 mvp = camera.getProjection() * camera.getView() * m_bsphere.model;
    m_bsphere.model = glm::scale(m_bsphere.model, glm::vec3(BSphereScale, BSphereScale, BSphereScale));
    glm::mat4 mvp = camera.getProjection() * camera.getView() * m_bsphere.model;
    m_DSPointLightPassTech.SetWVP(mvp);
    m_bsphere.Render();
  }
}

void directLightPass()
{
  // Directional Light Pass
  m_DSDirLightPassTech.Start();
  m_quad.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 8.0, 16.0));
  m_DSDirLightPassTech.SetEyeWorldPos(glm::vec3(camera.x(), camera.y(), camera.z()));
  glm::mat4 WVP = glm::mat4(1.0f);
  m_DSDirLightPassTech.SetWVP(WVP);
  m_quad.Render();
}

void render()
{
  g_pass.Start();
  g_buffer.BindForWriting();

  glDepthMask(GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  //glDepthFunc(GL_LESS);
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_FRONT);
  //glClearColor(0.f, 0.f, 0.5f, 0.f);

  for(int i = 0; i < sizeof(box)/sizeof(box[0]); i++)
  {
    if(box[i].ison())
    {
      glm::mat4 mvp = camera.getProjection() * camera.getView() * box[i].model;
      //glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
      g_pass.SetMVP(mvp);
      g_pass.SetModel(box[i].model);
      box[i].Render();
    }
  }

  // When we get here the depth buffer is already populated and the stencil pass
  // depends on it, but it does not write to it.
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);

  beginLightPass();
  pointLightPass();
  directLightPass();

  /*
  if(platform.ison())
  {
    glm::mat4 mvp = camera.getProjection() * camera.getView() * platform.model;
    //glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    g_pass.SetMVP(mvp);
    g_pass.SetModel(platform.model);
    platform.Render(loc_position, loc_tex, loc_normal);
  }
  */
  //glEnableVertexAttribArray(0);
  //glEnableVertexAttribArray(1);
  //glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(V2), 0);
  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(V2), (void*)offsetof(V2,color));
  //glDrawArrays(GL_TRIANGLES, 0, 36);
  //glDisableVertexAttribArray(0);
  //glDisableVertexAttribArray(1);

  /*
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  g_buffer.BindForReading();
  GLint HalfWidth = (GLint)(width/2.0f);
  GLint HalfHeight = (GLint)(height/2.0f);

  g_buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
  glBlitFramebuffer(0, 0, width, height, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  g_buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  glBlitFramebuffer(0, 0, width, height, 0, HalfHeight, HalfWidth, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  g_buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  glBlitFramebuffer(0, 0, width, height, HalfWidth, HalfHeight, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  g_buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
  glBlitFramebuffer(0, 0, width, height, HalfWidth, 0, width, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
  */
  

  /*///////////////////////////////////////////////////////////////////////////////////////
    // Get world trans
    glm::mat4 ScaleTrans, RotateTrans, TranslationTrans, CameraTranslationTrans, CameraRotateTrans;

    ScaleTrans.InitScaleTransform(BSphereScale, BSphereScale, BSphereScale);
    RotateTrans.InitRotateTransform(0.0, 0.0, 0.0);
    TranslationTrans.InitTranslationTransform(worldPos.x, worldPos.y, worldPos.z);

    glm::mat4 m_Wtransformation = TranslationTrans * RotateTrans * ScaleTrans;

    // get view trans

    CameraTranslationTrans.InitTranslationTransform(-camera.x(), -camera.y(), -camera.z());

    glm::vec3 focus = camera.getFocus();
    CameraRotateTrans.InitCameraTransform(focus, (0, 1, 0));

    glm::mat4 m_Vtransformation = CameraRotateTrans * CameraTranslationTrans;

    // Get VP Trans
    Matrix4f PersProjTrans;
    PersProjTrans.InitPersProjTransform(m_persProjInfo);
    
    glm::mat4 m_VPtransformation = PersProjTrans * m_Vtransformation;

    glm::mat4 m_WVPtransformation = m_VPtransformation * m_Wtransformation;
  *////////////////////////////////////////////////////////////////////////////////////////////////////////
    
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
        m_pointLight[0].Position = glm::vec3(m_pointLight[0].Position.x+2, m_pointLight[0].Position.y, m_pointLight[0].Position.z);
        printf("Location: %f, %f, %f\n", m_pointLight[0].Position.x, m_pointLight[0].Position.y, m_pointLight[0].Position.z);
      }
      if(e.key.keysym.sym == SDLK_n)
      {
        m_pointLight[0].Position = glm::vec3(m_pointLight[0].Position.x-2, m_pointLight[0].Position.y, m_pointLight[0].Position.z);
        printf("Location: %f, %f, %f\n", m_pointLight[0].Position.x, m_pointLight[0].Position.y, m_pointLight[0].Position.z);
      }
      if(e.key.keysym.sym == SDLK_b)
      {
        m_pointLight[0].Position = glm::vec3(m_pointLight[0].Position.x, m_pointLight[0].Position.y+2, m_pointLight[0].Position.z);
        printf("Location: %f, %f, %f\n", m_pointLight[0].Position.x, m_pointLight[0].Position.y, m_pointLight[0].Position.z);
      }
      if(e.key.keysym.sym == SDLK_v)
      {
        m_pointLight[0].Position = glm::vec3(m_pointLight[0].Position.x, m_pointLight[0].Position.y-2, m_pointLight[0].Position.z);
        printf("Location: %f, %f, %f\n", m_pointLight[0].Position.x, m_pointLight[0].Position.y, m_pointLight[0].Position.z);
      }
      if(e.key.keysym.sym == SDLK_c)
      {
        m_pointLight[0].Position = glm::vec3(m_pointLight[0].Position.x, m_pointLight[0].Position.y, m_pointLight[0].Position.z+2);
        printf("Location: %f, %f, %f\n", m_pointLight[0].Position.x, m_pointLight[0].Position.y, m_pointLight[0].Position.z);
      }
      if(e.key.keysym.sym == SDLK_x)
      {
        m_pointLight[0].Position = glm::vec3(m_pointLight[0].Position.x, m_pointLight[0].Position.y, m_pointLight[0].Position.z-2);
        printf("Location: %f, %f, %f\n", m_pointLight[0].Position.x, m_pointLight[0].Position.y, m_pointLight[0].Position.z);
      }

      if (e.key.keysym.sym == SDLK_MINUS)
      {
        m_dirLight.DiffuseIntensity += 0.1f;
      }
      if (e.key.keysym.sym == SDLK_EQUALS)
      {
        m_dirLight.DiffuseIntensity -= 0.1f;
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

float getDT()
{
  float ret;
  t2 = std::chrono::high_resolution_clock::now();
  ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
  t1 = std::chrono::high_resolution_clock::now();
  return ret;
}
