#include <SDL2/SDL.h>
#include <string>
using namespace std;

#include <camera.h>

class Window
{
  public:
    Window();
    ~Window();
    bool Initilize(string name, int* height, int* width);
    void Swap();

  private:
    SDL_Window* gWindow;
    SDL_GLContext gContext;
};
