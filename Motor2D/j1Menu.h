#ifndef __j1MENU_H__
#define __j1MENU_H__

#include "j1Module.h"
#include "p2List.h"

struct SDL_Texture;
class GuiImage;
class GuiText;
class Image;
class Label;
class Button;
class Window;
class Slider;

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

	void CreateMenu();
	void CreateSettings();
	void CreateCredits();
	void CleanUI();
	void SetUpMenu();

	void GUICallback(UI_Element* element);

	int currmap = 1;
	bool Started = false;

private:

	SDL_Texture* debug_tex;
	GuiImage* banner;
	//	GuiText* text;
	Image* Bg_ui_image = nullptr;
	Label* text = nullptr;
	Image* Title_ui = nullptr;

	Label* text_volum = nullptr;
	Label* min_vol = nullptr;
	Label* max_vol = nullptr;
	Label* curr_vol = nullptr;

	Label* text_frames = nullptr;
	Label* min_frames = nullptr;
	Label* max_frames = nullptr;
	Label* curr_frames = nullptr;

	Slider* Slider_Volume = nullptr;
	Slider* Slider_Frames = nullptr;

	Image* Slider_Image_Volum = nullptr;
	Image* Slider_Image_Frames = nullptr;
	Image* Button_Image_Volum = nullptr;
	Image* Button_Image_Frames = nullptr;

	Button* Login = nullptr;
	Button* Quit = nullptr;
	Button* Settings = nullptr;
	Button* Credits = nullptr;
	Button* Load_But = nullptr;

	Button* Return = nullptr;

	Window* window = nullptr;

	Button* Name = nullptr;

	bool quit_bool = false;
	bool settings_bool = false;
	bool credits_bool = false;
	bool return_menu = false;
	bool load_But_bool = false;
	bool clean_menu = false;

	bool can_quit = true;

	int GUI_Speed = 200;
public:
	uint tab_button = 0;
	bool StartGame = false;
	bool need_setup = true;

	bool debug = false;
};

#endif // __j1SCENE_H__
