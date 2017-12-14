#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2List.h"

struct SDL_Texture;

class GuiImage;
class GuiText;
class Image;
class Label;
class Button;
class Window;



class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

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

	bool LoadScene(int map = -1, bool is_load = false);
	void FreeScene();

	// Load
	bool Load(pugi::xml_node&  savegame);

	//Save
	bool Save(pugi::xml_node& data) const;

	void LoadWalkabilityMap();

	void FindEntities();
	
	void CreatePauseMenu();
	void DeletePauseMenu();

	void GUICallback(UI_Element* element);
	void OpenIngameMenu();

public:

	int currmap = 1;

	bool Photo_mode = false;
	bool Pause = false;
	bool next_map = false;
	bool reload_map = false;

private:

	uint width_map;

	int  sec, mins, hour, day, month, year;
	int last_frame_sec = -1;
	int photos_this_sec = 0;


	p2List<p2SString>  MapsList_String;
	p2List_item<p2SString>* CurrentMap = nullptr;

	Window* window = nullptr;
	Button* Return = nullptr;
	Button* Cancel = nullptr;
	bool return_menu = false;
	bool in_game_menu = false;


public:
	bool Quit = false;
};

#endif // __j1SCENE_H__