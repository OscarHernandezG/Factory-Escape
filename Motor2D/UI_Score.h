#pragma once
#include "UI.h"
#include "j1Textures.h"
#include "SDL/include/SDL.h"
#include "p2SString.h"

struct SDL_Texture;


class Score : public UI_Element
{
private:



public:
	Score(int x, int y);
	~Score();

	bool Update(float dt);

	bool CleanUp();

	bool LoadImageA(char* path, float scale = 1);

	bool MouseOnRect();

	bool MouseClick();

public:

	SDL_Texture* image = nullptr;

	SDL_Rect rect{ 0,0,0,0 };

	p2SString imagen;

	float scale = 1;

	bool draw = true;
};


