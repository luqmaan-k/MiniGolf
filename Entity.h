#pragma once

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "MathVector.h"

class Entity
{
public:
    Entity(Mathvector p_pos,Mathvector p_scale,SDL_Texture* p_tex);

//GETTERS
    SDL_Texture* getTex(){return texture;}
    SDL_Rect getcurFrame(){return curFrame;}
    Mathvector& getPos(){return pos;}
    Mathvector& getScale(){return scale;}
	float getAngle(){return angle;}
//SETTERS
	void setPos(float x, float y);
	void setScale(float w, float h);
	void setAngle(float angle);
	void settex(SDL_Texture* p_texture);

protected:

private:
    SDL_Texture* texture;

    SDL_Rect curFrame;

    Mathvector pos;
    Mathvector scale;

    float angle;
};
