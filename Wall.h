#pragma once
#include "Entity.h"
#include "MathVector.h"

class Wall : public Entity
{
    public:
        Wall(Mathvector p_pos,Mathvector p_scale,SDL_Texture* p_tex);

    protected:

    private:
};
