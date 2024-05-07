#include "Entity.h"
#include "MathVector.h"

Entity::Entity(Mathvector p_pos,Mathvector p_scale,SDL_Texture* p_texture):texture(p_texture),pos(p_pos),scale(p_scale),angle(0)
{
    curFrame.x=0;
    curFrame.y=0;
    SDL_QueryTexture(p_texture,NULL,NULL,&curFrame.w,&curFrame.h);
    curFrame.w*=p_scale.x;
    curFrame.h*=p_scale.y;
}

void Entity::setPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

void Entity::setAngle(float p_angle)
{
	angle = p_angle;
}

void Entity::setScale(float w, float h)
{
	scale.x = w;
	scale.y = h;
}

void Entity::settex(SDL_Texture* p_texture)
{
    texture=p_texture;
    SDL_QueryTexture(p_texture,NULL,NULL,&curFrame.w,&curFrame.h);
    curFrame.w*=scale.x;
    curFrame.h*=scale.y;
}
