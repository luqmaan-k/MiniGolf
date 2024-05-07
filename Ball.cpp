#include "Ball.h"

Ball::Ball(Mathvector p_pos,Mathvector p_scale,SDL_Texture* p_tex,SDL_Texture* p_arrow,SDL_Texture* p_powerbarbg,SDL_Texture* p_powerbarfg,int p_gameboundary_lwidth,int p_gameboundary_rwidth,int p_gameboundary_uheight,int p_gameboundary_dheight):Entity(p_pos,p_scale,p_tex),gameboundary_lwidth(p_gameboundary_lwidth),gameboundary_rwidth(p_gameboundary_rwidth),gameboundary_uheight(p_gameboundary_uheight),gameboundary_dheight(p_gameboundary_dheight),Movable(true),Swung(false),strokes(0),dirX(0),dirY(0),win(false),friction(0.001),arrow(Entity(Mathvector(-64,-64),Mathvector(1,1),p_arrow))
{
    powerbar.push_back(Entity(Mathvector(-64,-64),Mathvector(1,1),p_powerbarbg));
    powerbar.push_back(Entity(Mathvector(-64,-64),Mathvector(1,1),p_powerbarfg));
}

void Ball::setVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
}

void Ball::setLaunchedVelocity(float x, float y)
{
    launchedVelocity.x = x;
    launchedVelocity.y = y;
}

void Ball::setInitialMousePos(float x, float y)
{
    initialMousePos.x = x;
    initialMousePos.y = y;
}

void Ball::setWin(bool p_win)
{
    win = p_win;
}

void Ball::update(double deltaTime, bool mouseDown, bool mousePressed, std::vector<Wall> walls, std::vector<Hole> holes)
{
    if(win)
    {
        if (getPos().x < target.x)
        {
            setPos(getPos().x += 0.1*deltaTime, getPos().y);
        }
        else if (getPos().x > target.x)
        {
            setPos(getPos().x -= 0.1*deltaTime, getPos().y);
        }
        if (getPos().y < target.y)
        {
            setPos(getPos().x, getPos().y += 0.1*deltaTime);
        }
        else if (getPos().y > target.y)
        {
            setPos(getPos().x, getPos().y -= 0.1*deltaTime);
        }
        setScale(getScale().x - 0.001*deltaTime, getScale().y - 0.001*deltaTime);
        return;
    }

    for (Hole h : holes)
    {
        if (getPos().x + 4 > h.getPos().x && getPos().x + 16 < h.getPos().x + 20 && getPos().y + 4 > h.getPos().y && getPos().y + 16 < h.getPos().y + 20)
        {
            setWin(true);
            target.x = h.getPos().x ;
            target.y = h.getPos().y + 3;
        }
    }

    if (mousePressed && Movable)
    {
        Swung = false;
        int mouseX = 0;
	    int mouseY = 0;
	    SDL_GetMouseState(&mouseX, &mouseY);
        setInitialMousePos(mouseX, mouseY);
    }

    if (mouseDown && Movable)
    {

        int mouseX = 0;
	    int mouseY = 0;
	    SDL_GetMouseState(&mouseX, &mouseY);
        setVelocity((mouseX - getInitialMousePos().x)/-150, (mouseY - getInitialMousePos().y)/-150);
        setLaunchedVelocity((mouseX - getInitialMousePos().x)/-150, (mouseY - getInitialMousePos().y)/-150);
        velocity1D = SDL_sqrt(SDL_pow(std::abs(getVelocity().x), 2) + SDL_pow(std::abs((getVelocity().y)), 2));
        launchedVelocity1D = velocity1D;

        arrow.setPos(getPos().x, getPos().y + 8 - 32);
        arrow.setAngle(SDL_atan2(velocity.y, velocity.x)*(180/3.1415) + 90);

        dirX = velocity.x/std::abs((velocity.x));
        dirY = velocity.y/std::abs((velocity.y));

        powerbar.at(0).setPos(getPos().x + 32 + 8, getPos().y - 32);
        powerbar.at(1).setPos(getPos().x + 32 + 8 + 4, getPos().y - 32 + 4 + 32 - 32*powerbar.at(1).getScale().y);
        if (velocity1D > 1)
        {
            velocity1D = 1;
            launchedVelocity1D = 1;
        }
        powerbar.at(1).setScale(1, velocity1D);
    }

    else
    {
        if (!Swung)
        {
            Swung = true;
            strokes++;
        }
        arrow.setPos(-64, -64);
        powerbar.at(0).setPos(-64, -64);
        powerbar.at(1).setPos(-64, -64);
        Movable = false;
        setPos(getPos().x + getVelocity().x*deltaTime, getPos().y + getVelocity().y*deltaTime);
        if (getVelocity().x > 0.0001 || getVelocity().x < -0.0001 || getVelocity().y > 0.0001 || getVelocity().y < -0.0001)
        {
            if (velocity1D > 0)
            {
                velocity1D -= friction*deltaTime;
            }
            else
            {
                velocity1D = 0;
            }
            velocity.x = (velocity1D/launchedVelocity1D)*std::abs((launchedVelocity.x))*dirX;
            velocity.y = (velocity1D/launchedVelocity1D)*std::abs((launchedVelocity.y))*dirY;


        }
        else
        {
            setVelocity(0,0);
            int mouseX = 0;
	        int mouseY = 0;
            SDL_GetMouseState(&mouseX, &mouseY);
            setInitialMousePos(mouseX, mouseY);
            Movable = true;
        }

        //right of screen
        if (getPos().x + getcurFrame().w > gameboundary_rwidth)
        {
            setVelocity(-std::abs((getVelocity().x)), getVelocity().y);
            dirX = -1;
        }
        //left of screen
        else if (getPos().x < gameboundary_lwidth)
        {
            setVelocity(std::abs((getVelocity().x)), getVelocity().y);
            dirX = 1;
        }
        //under the screen
        else if (getPos().y + getcurFrame().h > gameboundary_dheight)
        {
            setVelocity(getVelocity().x, -std::abs( (getVelocity().y) ));
            dirY = -1;
        }
        //over the screen
        else if (getPos().y < gameboundary_uheight)
        {
            setVelocity(getVelocity().x, std::abs((getVelocity().y)));
            dirY = 1;
        }

        //Wall collision
        for (Wall& w : walls)
	    {
		    float newX = getPos().x + getVelocity().x*deltaTime;
            float newY = getPos().y;
            if (newX + 16 > w.getPos().x && newX < w.getPos().x + w.getcurFrame().w && newY + 16 > w.getPos().y && newY < w.getPos().y + w.getcurFrame().h - 3)
            {
                setVelocity(getVelocity().x*-1, getVelocity().y);
                dirX *= -1;
            }

            newX = getPos().x;
            newY = getPos().y + getVelocity().y*deltaTime;
            if (newX + 16 > w.getPos().x && newX < w.getPos().x + w.getcurFrame().w && newY + 16 > w.getPos().y && newY < w.getPos().y + w.getcurFrame().h - 3)
            {
                setVelocity(getVelocity().x, getVelocity().y*-1);
                dirY *= -1;
            }
	    }
    }
}
