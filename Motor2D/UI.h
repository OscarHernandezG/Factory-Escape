#ifndef __UI_H__
#define __UI_H__

#include "p2Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum GUI_TYPE {
	NO_TYPE_GUI,

	IMAGE,
	BUTTON,
	LABEL,
	WINDOW,
	SLIDER,
	SCORE,
	IMPUT_TEXT
};

class UI_Element
{
protected:
	

public:
	GUI_TYPE type = NO_TYPE_GUI;
	fPoint position;
	fPoint original_pos;

	SDL_Texture* atlas = nullptr;
	SDL_Rect debug_UI;

public:

	//Constructor
	UI_Element() {}
	UI_Element(int x, int y);

	//Destructor
	virtual ~UI_Element(){}

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called each loop iteration
	virtual bool Draw(SDL_Texture* sprites);


	// Called before quitting
	virtual bool CleanUp();

	virtual bool MouseOnRect();


};

#endif //__UI__H