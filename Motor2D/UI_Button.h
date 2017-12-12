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

enum BUTTON_NAME {

	NONE_NAME_BUT,
	MANAGE_ACCOUNT,
	COMMUNITY_SITE,
	NAME,
	PASWORD,
	LOGIN,
	CINEMATICS,
	CREDITS,
	TERMS_OF_USE,
	QUIT,

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

	void DefineButton(char* path, char* text, char* textesp = nullptr, BUTTON_TYPE type = NONE_TYPE_BUT);

public:
	Image* MouseClick = nullptr;

	Label* label = nullptr;
	Image* image = nullptr;
	BUTTON_TYPE Button_type = NONE_TYPE_BUT;
	BUTTON_NAME button_name = NONE_NAME_BUT;
	bool clicked = false;
	SDL_Rect rect;
	uint TAB = 0u;

	Image* MouseHovering = nullptr;


	/*Animation on_click;
	Aniamtion mouse_on_rect;*/

};


