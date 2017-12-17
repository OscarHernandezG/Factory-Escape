#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "UI.h"
#include "p2List.h"
#include "SDL/include/SDL.h"

#define CURSOR_WIDTH 2

#define MAX_UI_ELEMENTS 20

struct SDL_Rect;

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	
	UI_Element* AdUIElement(int x, int y, GUI_TYPE type);
	UI_Element* AdHUDElement(int x, int y, GUI_TYPE type);

	SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
public:
	p2List<UI_Element*> ui_elements;
	p2List<UI_Element*> hud_elements;

	SDL_Rect button_idle;
	SDL_Rect button_hovering;
	SDL_Rect button_onclick;
};

#endif // __j1GUI_H__