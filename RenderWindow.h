#pragma once

#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "Entity.h"

class RenderWindow
{
public:
    RenderWindow(const char* p_title,int p_w,int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);
	void cleanUp();
	void clear();
    void display();

    void render(Entity& p_entity);
    void rendertexture(int x, int y, SDL_Texture* p_tex);
    void rendertexturecenter(int x, int y, SDL_Texture* p_tex);
    void rendertext(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
    void rendertextcenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
protected:

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};
