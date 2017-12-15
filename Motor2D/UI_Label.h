#pragma once

#include "UI.h"
#include "p2SString.h"
#include "p2List.h"
#include "SDL\include\SDL.h"

struct TextInfo {

	SDL_Texture* tex = nullptr;
	SDL_Rect rect{ 0,0,0,0 };

	p2SString text;
};

class Label : public UI_Element
{
private:

public:

	Label(int x, int y);
	~Label();

	bool Update(float dt);
	bool CleanUp();

	bool SetText(char* text, int font = 0);


public:

	TextInfo text_info;

};


