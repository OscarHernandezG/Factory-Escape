#pragma once
#include "UI.h"
#include "j1Textures.h"
#include "SDL/include/SDL.h"
#include "p2SString.h"

struct SDL_Texture;


class Image : public UI_Element
{
private:



public:
	Image(int x, int y);
	~Image();

	bool Update(float dt);

	bool CleanUp();

	bool LoadUI_Image(SDL_Rect image_rect, float scale = 1);

	bool LoadUI_Image(char* path, float scale = 1);


public:

	SDL_Rect image{ 0,0,0,0 };	


	SDL_Texture* texture = nullptr;
	SDL_Texture* text = nullptr;

//	p2SString imagen;

	float scale = 1;

	bool draw = true;
	bool using_atlas = true;
};


