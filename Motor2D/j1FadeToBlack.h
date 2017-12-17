#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();
	~j1FadeToBlack();

	bool Start();
	bool Update(float dt);
	bool FadeToBlack(j1Module* module_off, j1Module* module_on, bool functions = true, float time = 2.0f, int LoadMap = -1);

	bool IsFading() const;

	j1Module* off;
	j1Module* on;
	bool black = true;

	int start_x = 130;
	int start_y = 110;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	uint w, h = 0;
	SDL_Rect screen;

	bool function = false;
	int map_num;

};

#endif //__MODULEFADETOBLACK_H__