#pragma once
#ifndef __J1GUIANIMATION_H__
#define __J1GUIANIMATION_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1GuiAnimation: public j1Module
{
public:
	j1GuiAnimation();
	~j1GuiAnimation();

	bool Start();
	bool Update(float dt);
	bool MoveToOrigin(j1Module* module_off, j1Module* module_on, float time = 2.0f);

	bool IsMoving() const;

	bool black = true;

	int start_x = 130;
	int start_y = 110;

private:

	enum move_gui
	{
		none,
		to_move,
		finished
	} current_step = move_gui::none;

	SDL_Rect screen;

};

#endif //__MODULEFADETOBLACK_H__