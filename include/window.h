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
    SDL_Cursor *init_system_cursor();
    void Swap();
    bool GetMode();
    void ChangeMode();

  private:
    SDL_Window* gWindow;
    SDL_GLContext gContext;
    bool mode;
};
