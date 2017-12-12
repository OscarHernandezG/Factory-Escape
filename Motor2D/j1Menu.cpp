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


	//Image* ESRB_notice = (Image*)App->gui->AdUIElement(21, 550, IMAGE);
	//ESRB_notice->LoadImageA("Homework/wow_ui/LOGIN/Glues-ESRBRating.png");


	//Button* Manage_Account = (Button*)App->gui->AdUIElement(17, 400, BUTTON);
	//Manage_Account->DefineButton("textures/Normal_But.png", "Manage Account", "Cuenta", INTERACTABLE);
	//Manage_Account->TAB = MANAGE_ACCOUNT;
	//

	//Button* Community_Site = (Button*)App->gui->AdUIElement(17, 500, BUTTON);
	//Community_Site->DefineButton("textures/Normal_But.png", "Community Site", "Pagina web", INTERACTABLE);
	//Community_Site->TAB = COMMUNITY_SITE;


	Login = (Button*)App->gui->AdUIElement(444, 380, BUTTON);
	Login->DefineButton("textures/Normal_But.png", "PLAY", "JUGAR", INTERACTABLE);
	int pos = 400;
	Login->AddListener(this);
	Login->TAB = LOGIN;


	/*Button* Cinematics = (Button*)App->gui->AdUIElement(872, pos, BUTTON);
	Cinematics->DefineButton("textures/Normal_But.png", "Cinematics", "Cinematicas", INTERACTABLE);
	pos += 39;
	Cinematics->TAB = CINEMATICS;*/


	//Button* Credits = (Button*)App->gui->AdUIElement(872, pos, BUTTON);
	//Credits->DefineButton("textures/Normal_But.png", "Credits", "Creditos", INTERACTABLE);
	//pos += 100;
	//Credits->TAB = CREDITS;


	/*Button* Terms_Of_Use = (Button*)App->gui->AdUIElement(872, pos, BUTTON);
	Terms_Of_Use->DefineButton("textures/Normal_But.png", "Terms Of Use", "Terminos de servicio", INTERACTABLE);
	pos = 538 - 20;
	Terms_Of_Use->TAB = TERMS_OF_USE;*/


	Quit = (Button*)App->gui->AdUIElement(872, pos, BUTTON);
	Quit->DefineButton("textures/Normal_But.png", "Quit", "Salir", INTERACTABLE);
	Quit->TAB = QUIT;
	Quit->AddListener(this);
	//ui_image->rect.add({ 0, 0, 1920, 1080 });

	/*ui_image->LoadImageA("gui/atlas.png");

	ui_image->rect.add({ 642, 169, 229, 69 });
	ui_image->rect.add({ 0,113,229,69 });*/

	//text = (Label*)App->gui->AdUIElement(0, 600, LABEL);
	//text->SetText("Version 2.0.12 (6546) (Release)");

	//text = (Label*)App->gui->AdUIElement(0, 610, LABEL);
	//text->SetText("Mar 30 2007");

	//text = (Label*)App->gui->AdUIElement(485, 650, LABEL);
	//text->SetText("Copyright 2004-2007  Blizzard Entretainment. All Rights Reserved.");

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
			if (tab_button > QUIT)
				tab_button = MANAGE_ACCOUNT;
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

}
