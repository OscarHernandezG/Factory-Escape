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
class Score;



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

	void DeleteMenu();

	void CreateSettingsMenu();

	void CreateSaveMenu();

	void CreateOptionsMenu();

	void GUICallback(UI_Element* element);
	void OpenInGameMenu();
	void OpenInGameSettings();
	void OpenInGameSave();
	void OpenInGameConfig();

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

	Button* Exit = nullptr;
	Button* Settings = nullptr;
	Button* Close = nullptr;

	Button* SaveLoad = nullptr;
	Button* Config = nullptr;
	Button* Back = nullptr;


	Button* Save1 = nullptr;
	Button* Save2 = nullptr;
	Button* Save3 = nullptr;

	Label* volume = nullptr;
	Label* frames = nullptr;
	


	bool return_menu = false;
	bool in_game_menu = false;
	bool in_game_settings = false;
	bool in_game_save = false;
	bool in_game_options = false;
	bool need_clean = false;

	Score* score = nullptr;
	uint score_nums = 0u;

public:
	bool Quit = false;
	bool can_quit = true;
};

#endif // __j1SCENE_H__