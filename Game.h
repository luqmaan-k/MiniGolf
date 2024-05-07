#pragma once

#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "RenderWindow.h"
#include "Ball.h"
#include "Hole.h"
#include "Wall.h"

class Game
{
public:

    Game();
    void Gameloop();
    bool init();
    void titlescreen();
    void displayscore();
    void updatelevel();
    void updatelevelgraphics();
    void loadlevel();
    void loadallTextures();
    void getoldhighscore();
    void getbestscore();
    void setnewhighscore();
    void quit();

protected:

private:

    bool gamequit;
    RenderWindow window;
    SDL_Event event;
    int level;
    int totlevels;
    int bestscores[3];

    bool mouseDown;
    bool mousePressed;

    Uint64 currentTick;
    Uint64 lastTick;
    double deltaTime;

    Ball ball;
    std::vector<Hole> holes;
    std::vector<Wall> walls;

    SDL_Color white;
    SDL_Color black;
    TTF_Font* font32 ;
    TTF_Font* font48 ;
    TTF_Font* font24 ;

    std::vector<SDL_Texture*> textures;
    SDL_Texture* background;
    SDL_Texture* temp;
};
