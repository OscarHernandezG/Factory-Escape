#ifndef __j1MENU_H__
#define __j1MENU_H__

#include "j1Module.h"
#include "p2List.h"

struct SDL_Texture;
class GuiImage;
class GuiText;
class Image;
class Label;


class j1Menu : public j1Module
{
public:

	j1Menu();

	// Destructor
	virtual ~j1Menu();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	int currmap = 1;

private:

	SDL_Texture* debug_tex;
	GuiImage* banner;
	//	GuiText* text;
	Image* ui_image = nullptr;
	Label* text = nullptr;


public:
	uint tab_button = 0;
	bool StartGame = false;
};

#endif // __j1SCENE_H__
