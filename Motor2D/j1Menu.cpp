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

	Title_ui = (Image*)App->gui->AdUIElement(0, 0, IMAGE);
	Title_ui->LoadImageA("textures/Title.png", 0.33f);


	Login = (Button*)App->gui->AdUIElement(500, 400, BUTTON);
	Login->DefineButton("textures/Normal_But.png", "PLAY", INTERACTABLE);
	Login->AddListener(this);
	Login->TAB = LOGIN;


	Quit = (Button*)App->gui->AdUIElement(1000, 600, BUTTON);
	Quit->DefineButton("textures/Normal_But.png", "QUIT", INTERACTABLE);
	Quit->TAB = QUIT;
	Quit->AddListener(this);

	Settings = (Button*)App->gui->AdUIElement(50, 600, BUTTON);
	Settings->DefineButton("textures/Normal_But.png", "SETTINGS", INTERACTABLE);
	Settings->TAB = SETTINGS;
	Settings->AddListener(this);

	Credits = (Button*)App->gui->AdUIElement(1000, 10, BUTTON);
	Credits->DefineButton("textures/Normal_But.png", "CREDITS", INTERACTABLE);
	Credits->TAB = CREDITS;
	Credits->AddListener(this);

	Load_But = (Button*)App->gui->AdUIElement(500, 600, BUTTON);
	Load_But->DefineButton("textures/Normal_But.png", "LOAD", INTERACTABLE);
	Load_But->TAB = LOAD;
	Load_But->AddListener(this);

	Name = (Button*)App->gui->AdUIElement(500, 500, BUTTON);
	Name->DefineButton("textures/Normal_But.png", "Name", INTERACTABLE);
	Name->TAB = LOAD;
	Name->AddListener(this);

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
		if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT)
			App->audio->VolumeUp();

		if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT)
			App->audio->VolumeDown();


		if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN) {
			tab_button++;
			if (tab_button > LOAD)
				tab_button = LOGIN;
		}
	}
	return true;
}

// Called each loop iteration
bool j1Menu::PostUpdate()
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);
	bool ret = true;
	
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit)
		ret = false;

	if (StartGame) {
		App->fade->FadeToBlack(this, App->scene, 1.0f);
		StartGame = false;
		Started = true;
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


	if (Login == element) {
		StartGame = true;
	}

	else if (Quit == element) {
		quit = true;
	}

	else if (Name == element) {
		
	}

}
