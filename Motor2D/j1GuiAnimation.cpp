#include "j1GuiAnimation.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "j1Menu.h"
//#include "UI.h"

#include "p2Defs.h"
#include "p2Log.h"
#include <math.h>

#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"


j1GuiAnimation::j1GuiAnimation()
{}

j1GuiAnimation::~j1GuiAnimation()
{}

// Load assets
bool j1GuiAnimation::Start()
{
/*	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	App->win->GetWindowSize(w, h);*/

	return true;
}

bool j1GuiAnimation::Update(float dt)
{
	if (current_step == move_gui::none)
		return true;


	switch (current_step)
	{
	case move_gui::to_move:
	{
	} break;
	}



	if (IsMoving()) {

	}
	return true;
}

bool j1GuiAnimation::MoveToOrigin(UI_Element* element)
{
	bool ret = true;

	//ui_elements_list->add(element);
	

	return ret;
}

bool j1GuiAnimation::IsMoving() const
{
	return current_step != move_gui::none;
}