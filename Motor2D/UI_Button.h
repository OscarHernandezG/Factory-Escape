#pragma once
#include "UI.h"
#include "UI_Label.h"
#include "UI_Image.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;


class Button : public UI_Element
{
private:

public:
	Button(int x, int y);

	~Button(){}

	bool Update(float dt);

	bool MouseOnRect();

	bool CleanUp();

	bool Define(SDL_Rect idle, SDL_Rect hovering, SDL_Rect oncick, char* text);

	void AddListener(j1Module* listener);
	void AddListener(p2List<j1Module*> listener);

	void DrebugDraw();

public:
	Image* MouseClick = nullptr;

	Label* label = nullptr;
	Image* image = nullptr;
	bool clicked = false;
	SDL_Rect rect;
	uint TAB = 0u;

	Image* MouseHovering = nullptr;

	p2List<j1Module*> listeners;


};