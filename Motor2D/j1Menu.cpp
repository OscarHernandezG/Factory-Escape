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
	LOG("Loading Scene");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool j1Menu::Start()
{

	Bg_ui_image = (Image*)App->gui->AdUIElement(0, 0, IMAGE);
	Bg_ui_image->LoadImageA("textures/Background_UI2.png");

	CreateMenu();

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
	if (!Started) {
		if (Quit->position.y > 600 || Settings->position.y > 600 || Load_But->position.y > 600) {
			Quit->position.y -= dt * 200;
			Settings->position.y -= dt * 200;
			Load_But->position.y -= dt * 200;
		}
		if (Login->position.y > 400)
			Login->position.y -= dt * 200;

		if (Credits->position.x > 1000)
			Credits->position.x -= dt * 200;

		if (Title_ui->position.x < 0)
			Title_ui->position.x += dt * 400;
		
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
	
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit_bool)
		ret = false;

	if (StartGame) {
		App->fade->FadeToBlack(this, App->scene, 1.0f);
		StartGame = false;
		Started = true;
	}

	if (settings_bool) {
		settings_bool = false;
		CleanMenu();
		CreateSettings();
	}

	if (credits_bool) {
		credits_bool = false;
		CleanMenu();
		CreateCredits();
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
		App->scene->Start();
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

}


void j1Menu::CreateMenu() {

	Title_ui = (Image*)App->gui->AdUIElement(-200, 0, IMAGE);
	Title_ui->LoadImageA("textures/Title.png", 0.33f);


	Login = (Button*)App->gui->AdUIElement(500, 700, BUTTON);//y = 400
	Login->DefineButton("textures/Normal_But.png", "PLAY", INTERACTABLE);
	Login->AddListener(this);
	Login->TAB = 1;


	Quit = (Button*)App->gui->AdUIElement(1000, 800, BUTTON); // y = 600
	Quit->DefineButton("textures/Normal_But.png", "QUIT", INTERACTABLE);
	Quit->TAB = 5;
	Quit->AddListener(this);

	Settings = (Button*)App->gui->AdUIElement(50, 800, BUTTON); //y = 600
	Settings->DefineButton("textures/Normal_But.png", "SETTINGS", INTERACTABLE);
	Settings->TAB = 2;
	Settings->AddListener(this);

	Credits = (Button*)App->gui->AdUIElement(1200, 10, BUTTON);//x = 1000
	Credits->DefineButton("textures/Normal_But.png", "CREDITS", INTERACTABLE);
	Credits->TAB = 4;
	Credits->AddListener(this);

	Load_But = (Button*)App->gui->AdUIElement(500, 800, BUTTON);//y = 600
	Load_But->DefineButton("textures/Normal_But.png", "LOAD", INTERACTABLE);
	Load_But->TAB = 3;
	Load_But->AddListener(this);

}
void j1Menu::CreateSettings() {

}
void j1Menu::CreateCredits() {

}
void j1Menu::CleanMenu() {
	for (p2List_item<UI_Element*>* iterator = App->gui->ui_elements.start; iterator != nullptr; iterator = iterator->next) {
		if (iterator->data != Bg_ui_image) {
			iterator->data->CleanUp();
			App->gui->ui_elements.del(iterator);
		}
	}
}