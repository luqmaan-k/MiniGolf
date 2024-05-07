#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>
#define WINDOWWIDTH 1280
#define WINDOWHEIGHT 640
Game::Game():gamequit(false),window(RenderWindow("Games title",WINDOWWIDTH,WINDOWHEIGHT)),level(0),totlevels(3),mouseDown(false),mousePressed(false),currentTick(SDL_GetPerformanceCounter()),lastTick(0),deltaTime(0),ball(Ball(Mathvector(32,32),Mathvector(1,1),window.loadTexture("res/gfx/ball.png"),window.loadTexture("res/gfx/arrow.png"),window.loadTexture("res/gfx/powermeter_bg.png"),window.loadTexture("res/gfx/powermeter_fg.png"),0,WINDOWWIDTH,0,WINDOWHEIGHT)),background(NULL)
{
    currentTick = SDL_GetPerformanceCounter();
    getbestscore();
}

void Game::Gameloop()
{
    while (!gamequit)
	{
        if (SDL_GetTicks() < 2000)
        {
		//Get our controls and events
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				gamequit = true;
				break;
			}
		}
		window.clear();
		window.rendertexture(0, 0, background);
		window.rendertextcenter(0, 0 + 3, "Welcome", font32, black);
		window.rendertextcenter(0, 0, "Welcome", font32, white);
		window.display();
        }
        else if(level == 0 )
        {
            titlescreen();
        }
        else
        {
        if(level>0 && level<=totlevels)
        {
            updatelevel();
            updatelevelgraphics();
        }
        if(level == totlevels+1)
        {
            level=0;
            loadlevel();
        }
        }
    }

}

bool Game::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
		std::cout << "SDL COULD NOT INITIALISE! SDL_ERROR: " << SDL_GetError() << std::endl;
        return false;
    }
	if (!(IMG_Init(IMG_INIT_PNG)))
    {
		std::cout << "SDL_IMG COULD NOT INITITALISE. Error: " << SDL_GetError() << std::endl;
        return false;
    }
	if (TTF_Init())
	{
		std::cout << "SDL_TTF COULD NOT INITIALISE. Error: " << SDL_GetError() << std::endl;
		return false;
    }
    font32 = TTF_OpenFont("res/font/font.ttf", 32);
    font48 = TTF_OpenFont("res/font/font.ttf", 48);
    font24 = TTF_OpenFont("res/font/font.ttf", 24);
    white = { 255, 255, 255 };
    black = {0,0,0};
    loadallTextures();
	return true;
}

void Game::titlescreen()
{
    lastTick = currentTick;
    currentTick = SDL_GetPerformanceCounter();
    deltaTime = (double)((currentTick - lastTick)*1000 / (double)SDL_GetPerformanceFrequency() );

    //Get our controls and events
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            gamequit = true;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                gamequit=true;
                break;
            }
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                level = 1;
                loadlevel();
            }
            break;
        }
    }
    window.clear();
    window.rendertexture(0, 0, background);
    window.rendertexturecenter(0, -50 + 20*SDL_sin(SDL_GetTicks()*(3.14/1500)), textures[11]);
    window.rendertextcenter(0, 150 + 3, "LEFT CLICK TO START", font48, black);
    window.rendertextcenter(0, 150, "LEFT CLICK TO START", font48, white);

    window.rendertextcenter(0, 200 + 3, "ESC TO EXIT", font32, black);
    window.rendertextcenter(0, 200, "ESC TO EXIT", font32, white);

    window.display();
}

void Game::displayscore()
{
    int starttime=SDL_GetTicks();
    window.rendertexturecenter(0,0,textures[16]);
    std::string StrokeText = std::to_string(ball.getStrokes());
    StrokeText = "STROKES: " + StrokeText;
    window.rendertextcenter(0 , -100 + 4 , StrokeText.c_str(), font48, black);
    window.rendertextcenter(0 , -100 , StrokeText.c_str(), font48, white);
    std::string BestScoreText = std::to_string(bestscores[level - 1]);
    BestScoreText = "BESTSCORE: " + BestScoreText;
    window.rendertextcenter(0 , 50 + 4 , BestScoreText.c_str() , font48, black);
    window.rendertextcenter(0 , 50 , BestScoreText.c_str() , font48, white);
    if(ball.getStrokes() < bestscores[level-1])
    {
        window.rendertextcenter(0, 100 + 4, "You scored a new best score!!!", font32, black);
        window.rendertextcenter(0, 100, "You scored a new best score!!!", font32, white);
        std::cout<<bestscores[level-1]<<" "<<ball.getStrokes()<<std::endl;
        bestscores[level-1]=ball.getStrokes();
        setnewhighscore();

    }
    window.display();
    while(SDL_GetTicks()<3000+starttime)
    {}
    ball.setStrokes(0);
}

void Game::updatelevel()
{
    lastTick = currentTick;
    currentTick = SDL_GetPerformanceCounter();
    deltaTime = (double)((currentTick - lastTick)*1000 / (double)SDL_GetPerformanceFrequency() );

    mousePressed = false;
    //Get our controls and events
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            gamequit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mouseDown = true;
                mousePressed = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mouseDown = false;
            }
            break;
        }
    }
    if(level>0 && level<totlevels+1)
    {
        ball.update(deltaTime,mouseDown,mousePressed,walls,holes);
        if(ball.getScale().x < -1 )
        {
            displayscore();
            level++;
            loadlevel();
        }
    }
}

void Game::updatelevelgraphics()
{
    window.clear();
    window.rendertexture(0,0,background);
    for(Hole& hole: holes)
    {
        window.render(hole);
    }
//render all the balls and arrows
    if(!ball.isWin())
    {
           window.rendertexture(ball.getPos().x,ball.getPos().y+4,textures[6]);
    }
    window.render(ball.getarrow());
    window.render(ball);
//render all the walls
    for(Wall& wall : walls)
    {
        window.render(wall);
    }
//render power bar
    window.render(ball.getPowerBar().at(0));
    window.render(ball.getPowerBar().at(1));
    window.rendertexture(ball.getPowerBar().at(0).getPos().x, ball.getPowerBar().at(0).getPos().y, textures[10]);
//render end of all levels
    std::string LevelText = ("LEVEL: " + std::to_string(level));
    std::string StrokeText = ("STROKES: " + std::to_string(ball.getStrokes()));
    window.rendertextcenter(0, -WINDOWHEIGHT/2 + 32/2 - 0 , LevelText.c_str(), font32, black);
    window.rendertextcenter(0, -WINDOWHEIGHT/2 + 32/2 - 4 , LevelText.c_str(), font32, white);
    
    window.rendertextcenter(0 , WINDOWHEIGHT/2 - 24/2 + 0 , StrokeText.c_str(), font24, black);
    window.rendertextcenter(0 , WINDOWHEIGHT/2 - 24/2 - 4 , StrokeText.c_str(), font24, white);

    window.display();
}

void Game::loadlevel()
{
    ball.setPos(0,0);
    ball.setVelocity(0,0);
    ball.setScale(1,1);
    ball.setWin(false);
    switch(level)
    {
    case 1:
        holes.clear();
        walls.clear();
        ball.setPos(640,584);
        holes.push_back(Hole(Mathvector(640,128),Mathvector(1,1),textures[15]));
        for(int i=0;i<WINDOWHEIGHT/32;i++)
        {
            walls.push_back(Wall(Mathvector(384,32*i),Mathvector(1,1),textures[2+(i%2)]));
            walls.push_back(Wall(Mathvector(864,32*i),Mathvector(1,1),textures[2+(i%2)]));
        }
        break;
    case 2:
        holes.clear();
        walls.clear();
        ball.setPos(153,313.5);
        holes.push_back(Hole(Mathvector(1196,310.500),Mathvector(1,1),textures[15]));
        for(int i=0;i<WINDOWWIDTH/32;i++)
        {
            walls.push_back(Wall(Mathvector(32*i,5*32),Mathvector(1,1),textures[2+(i%2)]));
            walls.push_back(Wall(Mathvector(32*i,(WINDOWHEIGHT/32-1-5)*32),Mathvector(1,1),textures[2+(i%2)]));
        }
        for(int i=0;i<7;i++)
        {
            walls.push_back(Wall(Mathvector(288+128*i,288),Mathvector(1,1),textures[4+(i%2)]));
        }
        break;
    case 3:
        holes.clear();
        walls.clear();
        ball.setPos(40,584);
        holes.push_back(Hole(Mathvector(1128,134),Mathvector(1,1),textures[15]));
        holes.push_back(Hole(Mathvector(1176,566),Mathvector(1,1),textures[15]));
        for(int i=0;i<6;i++)
            walls.push_back(Wall(Mathvector(1024+32*i,224),Mathvector(1,1),textures[3]));
        walls.push_back(Wall(Mathvector(320,384),Mathvector(1,1),textures[3]));
        for(int i=0;i<5;i++)
            walls.push_back(Wall(Mathvector(0+32*i,416),Mathvector(1,1),textures[2]));
        for(int i=0;i<3;i++)
            walls.push_back(Wall(Mathvector(320,416+32*i),Mathvector(1,1),textures[3]));
        walls.push_back(Wall(Mathvector(1024,480),Mathvector(1,1),textures[2]));
        walls.push_back(Wall(Mathvector(320,512),Mathvector(1,1),textures[3]));
        walls.push_back(Wall(Mathvector(1024,512),Mathvector(1,1),textures[2]));
        walls.push_back(Wall(Mathvector(320,544),Mathvector(1,1),textures[3]));
        walls.push_back(Wall(Mathvector(1024,544),Mathvector(1,1),textures[2]));
        walls.push_back(Wall(Mathvector(320,576),Mathvector(1,1),textures[3]));
        walls.push_back(Wall(Mathvector(320,608),Mathvector(1,1),textures[3]));

        walls.push_back(Wall(Mathvector(960,0),Mathvector(1,1),textures[4]));
        walls.push_back(Wall(Mathvector(512,96),Mathvector(1,1),textures[5]));
        walls.push_back(Wall(Mathvector(960,128),Mathvector(1,1),textures[4]));
        walls.push_back(Wall(Mathvector(960,192),Mathvector(1,1),textures[4]));
        walls.push_back(Wall(Mathvector(96,256),Mathvector(1,1),textures[5]));
        walls.push_back(Wall(Mathvector(640,384),Mathvector(1,1),textures[4]));
        walls.push_back(Wall(Mathvector(1024,416),Mathvector(1,1),textures[5]));
        walls.push_back(Wall(Mathvector(1088,416),Mathvector(1,1),textures[5]));
        walls.push_back(Wall(Mathvector(1152,416),Mathvector(1,1),textures[5]));
        walls.push_back(Wall(Mathvector(1216,416),Mathvector(1,1),textures[5]));
        break;
    }
}

void Game::loadallTextures()
{
    background=window.loadTexture("res/gfx/bg.png");
    textures.push_back(window.loadTexture("res/gfx/bg.png"));
    textures.push_back(window.loadTexture("res/gfx/ball.png"));//1
    textures.push_back(window.loadTexture("res/gfx/tile32_dark.png"));//2
    textures.push_back(window.loadTexture("res/gfx/tile32_light.png"));//3
    textures.push_back(window.loadTexture("res/gfx/tile64_dark.png"));//4
    textures.push_back(window.loadTexture("res/gfx/tile64_light.png"));//5
    textures.push_back(window.loadTexture("res/gfx/ball_shadow.png"));//6
    textures.push_back(window.loadTexture("res/gfx/UI_bg.png"));//7
    textures.push_back(window.loadTexture("res/gfx/levelText_bg.png"));//8
    textures.push_back(window.loadTexture("res/gfx/powermeter_bg.png"));//9
    textures.push_back(window.loadTexture("res/gfx/powermeter_overlay.png"));//10
    textures.push_back(window.loadTexture("res/gfx/logo.png"));//11
    textures.push_back(window.loadTexture("res/gfx/click2start.png"));//12
    textures.push_back(window.loadTexture("res/gfx/end.png"));//13
    textures.push_back(window.loadTexture("res/gfx/splashbg.png"));//14
    textures.push_back(window.loadTexture("res/gfx/hole.png"));//15
    textures.push_back(window.loadTexture("res/gfx/greyoverlay.png"));//16

}

void Game::quit()
{
    window.cleanUp();
	TTF_CloseFont(font32);
	TTF_CloseFont(font48);
	TTF_CloseFont(font24);
	SDL_Quit();
	TTF_Quit();
}

void Game::getbestscore()
{
    std::ifstream file("bestscores.bin",std::ios::in|std::ios::binary);
    file.read((char*)bestscores,sizeof(int)*3);
    file.close();
}

void Game::setnewhighscore()
{
    std::ofstream file("bestscores.bin",std::ios::out|std::ios::binary);
    file.write((char*)bestscores,sizeof(int)*3);
    file.close();
}
