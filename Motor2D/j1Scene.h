#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2List.h"
#include "j1Timer.h"

struct SDL_Texture;

class GuiImage;
class GuiText;
class Image;
class Label;
class Button;
class Window;
class Score;
class Slider;


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
	void FreeScene(bool is_load = false);

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
	bool draw_ui = true;
	bool Pause = false;
	bool next_map = false;
	bool reload_map = false;
	bool click_PauseButt = false;

	bool need_load_scene = false;

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


	Label* text_volum = nullptr;
	Label* min_vol = nullptr;
	Label* max_vol = nullptr;
	Label* curr_vol = nullptr;

	Label* text_frames = nullptr;
	Label* min_frames = nullptr;
	Label* max_frames = nullptr;
	Label* curr_frames = nullptr;

	Slider* volume = nullptr;
	Slider* frames = nullptr;


	bool return_menu = false;
	bool in_game_menu = false;
	bool in_game_settings = false;
	bool in_game_save = false;
	bool in_game_options = false;
	bool need_clean = false;

	bool need_save = false;
	bool saved = false;
	int save_game = 0;

	Score* score = nullptr;

	Button* pause_butt = nullptr;

	Label* Timer = nullptr;
	

public:
	uint score_nums = 0u;
	bool change_score = false;
	bool score_anim = false;
	bool Quit = false;
	bool can_quit = true;

	j1Timer Timer_play;
	uint StartPause, ClosePause = 0u;

};

#endif // __j1SCENE_H__