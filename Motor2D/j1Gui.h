#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "UI.h"
#include "p2List.h"

#define CURSOR_WIDTH 2

#define MAX_UI_ELEMENTS 20

// TODO 1: Create your structure of classes


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

	const SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
public:
	p2List<UI_Element*> ui_elements;

};

#endif // __j1GUI_H__