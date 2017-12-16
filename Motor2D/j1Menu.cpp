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
#include "UI.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Label.h"
#include "j1FadeToBlack.h"
#include "j1Menu.h"
#include "j1GuiAnimation.h"
#include "UI_Window.h"
#include "UI_Slider.h"
#include "j1Fonts.h"

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

	if (!need_setup && !quit_bool && !settings_bool && !credits_bool && !load_But_bool && !clean_menu) {
		if (Quit->position.y > 600 || Settings->position.y > 600 || Load_But->position.y > 600) {
			Quit->position.y -= dt * GUI_Speed;
			Settings->position.y -= dt * GUI_Speed;
			Load_But->position.y -= dt * GUI_Speed;
		}
		if (Login->position.y > 400)
			Login->position.y -= dt * GUI_Speed;

		if (Credits->position.x > 1000)
			Credits->position.x -= dt * GUI_Speed;

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
	/*if (clean_menu) {
		if (Quit->position.y < 800 || Settings->position.y < 800 || Load_But->position.y < 800) {
			Quit->position.y += dt * 200;
			Settings->position.y += dt * 200;
			Load_But->position.y += dt * 200;
		}
		if (Login->position.y < 800)
			Login->position.y += dt * 200;

		if (Credits->position.x < 1300)
			Credits->position.x += dt * 200;

		if (Title_ui->position.x > -4000)
			Title_ui->position.x -= dt * 400;
		//CleanMenu();

	}
	*/

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
		CleanMenu();
		CreateMenu();
		can_quit = true;
	}

	if (StartGame) {
		App->fade->FadeToBlack(this, App->scene, 1.0f);
		StartGame = false;
		Started = true;
	}
	
	else if (settings_bool) {
		can_quit = false;
		settings_bool = false;
//		clean_menu = true;
		CleanMenu();
		CreateSettings();
	}

	else if (credits_bool) {
		can_quit = false;
		credits_bool = false;
		CleanMenu();
		CreateCredits();
	}

	else if (return_menu) {
		return_menu = false;
		CleanMenu();
		CreateMenu();
		can_quit = true;
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
	//	App->scene->Start();
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

	else if (Credits == element)
		credits_bool = true;

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
		sprintf_s(vol_text, 4, "%.0f", volume*100);
		curr_vol->SetText(vol_text);
		LOG("%f", volume);
	}

}

void j1Menu::SetUpMenu() {
	need_setup = false;
	/*Bg_ui_image = (Image*)App->gui->AdUIElement(0, 0, IMAGE);
	Bg_ui_image->LoadUI_Image("textures/Background_UI.png");*/

	CreateMenu();
}


void j1Menu::CreateMenu() {

	Bg_ui_image = (Image*)App->gui->AdUIElement(0, 0, IMAGE);
	Bg_ui_image->LoadUI_Image("textures/Background_UI.png");


	Title_ui = (Image*)App->gui->AdUIElement(-200, 0, IMAGE);
	Title_ui->LoadUI_Image("textures/Title.png", 0.33f);
	App->gui_animation->MoveToOrigin(Title_ui);

	Login = (Button*)App->gui->AdUIElement(500, 700, BUTTON);//y = 400
	Login->Define("textures/Normal_But.png", "PLAY");
	Login->AddListener(this);
	Login->TAB = 1;
	App->gui_animation->MoveToOrigin(Login);

	Quit = (Button*)App->gui->AdUIElement(1000, 800, BUTTON); // y = 600
	Quit->Define("textures/Normal_But.png", "EXIT");
	Quit->TAB = 5;
	Quit->AddListener(this);
	App->gui_animation->MoveToOrigin(Quit);

	Settings = (Button*)App->gui->AdUIElement(50, 800, BUTTON); //y = 600
	Settings->Define("textures/Normal_But.png", "SETTINGS");
	Settings->TAB = 2;
	Settings->AddListener(this);
	App->gui_animation->MoveToOrigin(Settings);

	Credits = (Button*)App->gui->AdUIElement(1200, 10, BUTTON);//x = 1000
	Credits->Define("textures/Normal_But.png", "CREDITS");
	Credits->TAB = 4;
	Credits->AddListener(this);
	App->gui_animation->MoveToOrigin(Credits);

	Load_But = (Button*)App->gui->AdUIElement(500, 800, BUTTON);//y = 600
	Load_But->Define("textures/Normal_But.png", "LOAD");
	Load_But->TAB = 3;
	Load_But->AddListener(this);
	App->gui_animation->MoveToOrigin(Load_But);

}
void j1Menu::CreateSettings() {

	Bg_ui_image = (Image*)App->gui->AdUIElement(0, 0, IMAGE);
	Bg_ui_image->LoadUI_Image("textures/Background_UI.png");

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
	//Slider_Volum->LoadImageA("textures/slider.png");
	App->gui_animation->MoveToOrigin(Slider_Volume);
	Slider_Volume->AddListener(this);
	Slider_Volume->SetRelativePos(App->audio->volume);

	Slider_Frames = (Slider*)App->gui->AdUIElement(400, 500, SLIDER);
	//Slider_Frames->LoadImageA("textures/Ball_slider.png");
	App->gui_animation->MoveToOrigin(Slider_Frames);
	Slider_Frames->AddListener(this);
	float frames = App->current_framerate_cap - 30;
	Slider_Frames->SetRelativePos((float)frames/210);



	Return = (Button*)App->gui->AdUIElement(900, 600, BUTTON); 
	Return->Define("textures/Normal_But.png", "RETURN");
	Return->TAB = -1;
	Return->AddListener(this);

}


void j1Menu::CreateCredits() {

}
void j1Menu::CleanMenu() {

	for (p2List_item<UI_Element*>* iterator = App->gui->ui_elements.start; iterator != nullptr; iterator = iterator->next) {
		if (iterator->data != Bg_ui_image) {
			iterator->data->CleanUp();
		}
	}
	App->gui->ui_elements.clear();
}