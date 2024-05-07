#pragma once
#include <iostream>
#include <vector>
#include <cmath>

#include <SDL.h>

#include "Entity.h"
#include "Hole.h"
#include "Wall.h"
#include "MathVector.h"

class Ball:public Entity
{
public:
    Ball(Mathvector p_pos,Mathvector p_scale,SDL_Texture* p_tex,SDL_Texture* p_arrow,SDL_Texture* p_powerbar,SDL_Texture* p_powerbarfg,int p_gameboundary_lwidth,int p_gameboundary_rwidth,int p_gameboundary_uheight,int p_gameboundary_dheight);
    Mathvector& getVelocity(){return velocity;}
    Mathvector& getInitialMousePos(){return initialMousePos;}
    Entity& getarrow(){return arrow;}
    std::vector<Entity> getPowerBar(){return powerbar;}
    int getStrokes(){return strokes;}
    bool isWin(){return win;}
    void setStrokes(int s){strokes=s;}
    void setWin(bool p_win);
    void setInitialMousePos(float x, float y);
    void setVelocity(float x, float y);
    void setLaunchedVelocity(float x, float y);
    void update(double deltaTime, bool mouseDown, bool mousePressed, std::vector<Wall> walls, std::vector<Hole> holes);

protected:

private:
    Mathvector velocity;
    Mathvector launchedVelocity;
    float velocity1D;
    float launchedVelocity1D;

    int gameboundary_lwidth;
    int gameboundary_rwidth;
    int gameboundary_uheight;
    int gameboundary_dheight;

    Mathvector target;

    Mathvector initialMousePos;

    bool Movable;
    bool Swung;

    int strokes;

    int dirX;
    int dirY;

    bool win;

    float friction;

    Entity arrow;
    std::vector<Entity> powerbar;
};
