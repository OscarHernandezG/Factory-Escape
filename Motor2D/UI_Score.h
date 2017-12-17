#pragma once
#include "UI.h"
#include "j1Textures.h"
#include "SDL/include/SDL.h"
#include "p2SString.h"
#include "UI_Label.h"
#include "UI_Image.h"

struct SDL_Texture;



class Score : public UI_Element
{
private:



public:
	Score(int x, int y);
	~Score();

	bool Update(float dt);

	bool CleanUp();


	bool Define(SDL_Rect rect, char* text,SDL_Rect rect2nd);

	void ChangeImage();

	bool ChangeLabel(char* text);


public:

	Image* image = nullptr;
	Image* image2nd = nullptr;
	Label* label = nullptr;

	SDL_Rect rect{ 0,0,0,0 };

	float scale = 1;

	bool draw = true;
	bool use_image1 = true;
};


