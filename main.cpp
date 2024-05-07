#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Game.h"
int main(int argc,char** argv)
{
    Game G;
    G.init();
    G.Gameloop();
    G.quit();
    return 0;
}
