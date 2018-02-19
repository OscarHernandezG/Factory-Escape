#include "p2Defs.h"
#include "p2Log.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Entities.h"
#include "j1Pathfinding.h"
#include "j1Gui.h"
#include "j1Fonts.h"

#include "UI.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Label.h"
#include "j1FadeToBlack.h"
#include "j1Menu.h"
#include "UI_Window.h"
#include "UI_Slider.h"
#include "UI_ImputText.h"

#include <time.h>
#include <chrono>
#include <ctime>
#include <iostream>




j1Menu::j1Menu() : j1Module()
{
	name.create("menu");
}

// Destructor
j1Menu::~j1Menu()
{}

// Called before render is available
bool j1Menu::Awake(pugi::xml_node& config)
{
	LOG("Loading Menu");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool j1Menu::Start()
{
	SetUpMenu();

	return true;
}

// Called each loop iteration
bool j1Menu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Menu::Update(float dt)
{

	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	if (need_setup)
		SetUpMenu();
	
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		debug = !debug;
	}

	if (!need_setup && !quit_bool && !settings_bool && !load_But_bool && !clean_menu) {
		if (Quit->position.y > 600 || Settings->position.y > 600 || Load_But->position.y > 600) {
			Quit->position.y -= dt * GUI_Speed;
			Settings->position.y -= dt * GUI_Speed;
			Load_But->position.y -= dt * GUI_Speed;
		}
		if (Login->position.y > 400)
			Login->position.y -= dt * GUI_Speed;

		if (Title_ui->position.x < 0)
			Title_ui->position.x += 2 * dt * GUI_Speed;

		if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT)
			App->audio->VolumeUp();

		if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT)
			App->audio->VolumeDown();


		if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN) {
			tab_button++;
			if (tab_button > 5)
				tab_button = 1;
		}
	}
	return true;
}

// Called each loop iteration
bool j1Menu::PostUpdate()
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);
	bool ret = true;
	
	if ((App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit_bool) && can_quit)
		ret = false;

	else if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && !can_quit) {
		CleanUI();
		CreateMenu();
		can_quit = true;
	}

	if (StartGame) {
		App->fade->FadeToBlack(this, App->scene);
		StartGame = false;
		Started = true;
	}
	
	else if (settings_bool) {
		can_quit = false;
		settings_bool = false;
		CleanUI();
		CreateSettings();
	}

	else if (return_menu) {
		return_menu = false;
		CleanUI();
		CreateMenu();
		can_quit = true;
	}

	else if (load_But_bool) {
		load_But_bool = false;
		CleanUI();
		CreateLoading();
		can_quit = false;
	}

	return ret;
}

// Called before quitting
bool j1Menu::CleanUp()
{
	LOG("Freeing Menu");
	
	for (p2List_item<UI_Element*>* iterator = App->gui->ui_elements.start; iterator != nullptr; iterator = iterator->next) {
		iterator->data->CleanUp();
	}
	App->gui->ui_elements.clear();

	if (active) {
		active = false;
		App->map->Start();
		App->entities->Start();
		App->scene->active = true;
		App->map->active = true;
		App->entities->active = true;
	}

	return true;
}

void j1Menu::GUICallback(UI_Element* element) {

	if (Login == element)
		StartGame = true;

	else if (Quit == element)
		quit_bool = true;

	else if (Settings == element)
		settings_bool = true;

	else if (Load_But == element)
		load_But_bool = true;

	else if (Return == element)
		return_menu = true;

	else if (Slider_Frames == element) {
		float frames = Slider_Frames->GetRelativePosition();
		frames = (frames * 210) + 30;
		App->framerate_cap = App->current_framerate_cap = frames;
		static char frames_text[5];

		sprintf_s(frames_text, 5, "%.0f", frames);
		curr_frames->SetText(frames_text);
		LOG("%f", frames);
	}
	else if (Slider_Volume == element) {
		float volume = Slider_Volume->GetRelativePosition();
		App->audio->SetVolume(volume);
		static char vol_text[4];
		sprintf_s(vol_text, 4, "%.0f", volume * 100);
		curr_vol->SetText(vol_text);
		LOG("%f", volume);
	}

	else if (Load1 == element) {
		need_load = true;
		load_map = 0;
	}
	else if (Load2 == element) {
		need_load = true;
		load_map = 1;
	}
	else if (Load3 == element) {
		need_load = true;
		load_map = 2;
	}

}

void j1Menu::SetUpMenu() {
	need_setup = false;
	Bg_ui_image = (Image*)App->gui->AdUIElement(0, 0, IMAGE);
	Bg_ui_image->LoadUI_Image("textures/Background_UI.png");

	ImputTextMenu = (ImputText*)App->gui->AdUIElement(1000, 10, IMPUT_TEXT);
	ImputTextMenu->Label_ImputText->SetText("Your Name");

	CreateMenu();
}


void j1Menu::CreateMenu() {


	Title_ui = (Image*)App->gui->AdUIElement(-200, 0, IMAGE);
	Title_ui->LoadUI_Image("textures/Title.png", 1);


	Login = (Button*)App->gui->AdUIElement(500, 700, BUTTON);//y = 400
	Login->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "PLAY");
	Login->AddListener(this);
	Login->TAB = 1;

	Quit = (Button*)App->gui->AdUIElement(1000, 800, BUTTON); // y = 600
	Quit->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "EXIT");
	Quit->TAB = 5;
	Quit->AddListener(this);

	Settings = (Button*)App->gui->AdUIElement(50, 800, BUTTON); //y = 600
	Settings->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "SETTINGS");
	Settings->TAB = 2;
	Settings->AddListener(this);

	Load_But = (Button*)App->gui->AdUIElement(500, 800, BUTTON);//y = 600
	Load_But->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "LOAD");
	Load_But->TAB = 3;
	Load_But->AddListener(this);

}

void j1Menu::CreateSettings() {

	text_volum = (Label*)App->gui->AdUIElement(500, 250, LABEL);
	text_volum->SetText("VOLUME");

	min_vol = (Label*)App->gui->AdUIElement(350, 300, LABEL);
	min_vol->SetText("0");

	max_vol = (Label*)App->gui->AdUIElement(750, 300, LABEL);
	max_vol->SetText("100");

	curr_vol = (Label*)App->gui->AdUIElement(625, 250, LABEL);
	static char vol_text[5];
	sprintf_s(vol_text, 5, "%.0f", App->audio->volume * 100);
	curr_vol->SetText(vol_text);
	

	text_frames = (Label*)App->gui->AdUIElement(500, 450, LABEL);
	text_frames->SetText("FPS");

	min_frames = (Label*)App->gui->AdUIElement(350, 500, LABEL);
	min_frames->SetText("30");

	max_frames = (Label*)App->gui->AdUIElement(750, 500, LABEL);
	max_frames->SetText("240");

	curr_frames = (Label*)App->gui->AdUIElement(625, 450, LABEL);
	static char fps_text[5];
	sprintf_s(fps_text, 5, "%i", App->current_framerate_cap);
	curr_frames->SetText(fps_text);

	Slider_Volume = (Slider*)App->gui->AdUIElement(400, 300, SLIDER);

	Slider_Volume->Define({ 31,275,321,20 }, { 260,198,25,25 });
	Slider_Volume->AddListener(this);
	Slider_Volume->SetRelativePos(App->audio->volume);


	Slider_Frames = (Slider*)App->gui->AdUIElement(400, 500, SLIDER);

	Slider_Frames->Define({ 31,275,321,20 }, { 260,198,25,25 });
	Slider_Frames->AddListener(this);
	float frames = App->current_framerate_cap - 30;
	Slider_Frames->SetRelativePos((float)frames/210);

	
	Return = (Button*)App->gui->AdUIElement(900, 600, BUTTON); 
	Return->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "RETURN");
	Return->TAB = -1;
	Return->AddListener(this);

}

void j1Menu::CreateLoading() {


	Load1 = (Button*)App->gui->AdUIElement(650, 150, BUTTON);
	Load1->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "LOAD GAME 1");
	Load1->TAB = -1;
	Load1->AddListener(this);

	Game1 = (Image*)App->gui->AdUIElement(300, 115, IMAGE);
	Game1->LoadUI_Image("savegame/savegame0-screenshot.png", 0.2f);
	


	Load2 = (Button*)App->gui->AdUIElement(650, 300, BUTTON);
	Load2->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "LOAD GAME 2");
	Load2->TAB = -1;
	Load2->AddListener(this);

	Game2 = (Image*)App->gui->AdUIElement(300, 265, IMAGE);
	Game2->LoadUI_Image("savegame/savegame1-screenshot.png", 0.2f);


	Load3 = (Button*)App->gui->AdUIElement(650, 450, BUTTON);
	Load3->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "LOAD GAME 3");
	Load3->TAB = -1;
	Load3->AddListener(this);

	Game3 = (Image*)App->gui->AdUIElement(300, 415, IMAGE);
	Game3->LoadUI_Image("savegame/savegame2-screenshot.png", 0.2f);
	

	Return = (Button*)App->gui->AdUIElement(900, 600, BUTTON);
	Return->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "RETURN");
	Return->TAB = -1;
	Return->AddListener(this);

}

void j1Menu::CleanUI() {

	for (p2List_item<UI_Element*>* iterator = App->gui->ui_elements.start; iterator != nullptr; iterator = iterator->next) {
	//	if (iterator->data != Bg_ui_image) {
			iterator->data->CleanUp();
//			App->gui->ui_elements.del(iterator);
	//	}
	}
	App->gui->ui_elements.clear();

	Bg_ui_image = (Image*)App->gui->AdUIElement(0, 0, IMAGE);
	Bg_ui_image->LoadUI_Image("textures/Background_UI.png");
}