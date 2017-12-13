#pragma once
#include "UI.h"
#include "UI_Label.h"
#include "UI_Image.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum BUTTON_TYPE {
	NONE_TYPE_BUT,
	
	INTERACTABLE,
	INPUT_BOX
};


class Button : public UI_Element
{
private:

public:
	Button(int x, int y);

	~Button(){}

	bool Update(float dt);

	bool Clicked();

	bool MouseOnRect();

	bool CleanUp();

	void DefineButton(char* path, char* text, BUTTON_TYPE type = NONE_TYPE_BUT);

	void AddListener(j1Module* listener);
	void AddListener(p2List<j1Module*> listener);

public:
	Image* MouseClick = nullptr;

	Label* label = nullptr;
	Image* image = nullptr;
	BUTTON_TYPE Button_type = NONE_TYPE_BUT;
	bool clicked = false;
	SDL_Rect rect;
	uint TAB = 0u;

	Image* MouseHovering = nullptr;

	p2List<j1Module*> listeners;


};


