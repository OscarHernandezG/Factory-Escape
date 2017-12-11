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
#include "J1Player.h"
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

	ui_image = (Image*)App->gui->AdUIElement(0, 0, IMAGE);
	ui_image->LoadImageA("Homework/wow_ui/login_background.png");

	Image* wow_logo = (Image*)App->gui->AdUIElement(0, 0, IMAGE);
	wow_logo->LoadImageA("Homework/wow_ui/COMMON/Glues-Logo-Left.png", 0.33f);
	wow_logo = (Image*)App->gui->AdUIElement(85, 0, IMAGE);
	wow_logo->LoadImageA("Homework/wow_ui/COMMON/Glues-Logo-Right.png", 0.33f);

	Image* ESRB_notice = (Image*)App->gui->AdUIElement(21, 415, IMAGE);
	ESRB_notice->LoadImageA("Homework/wow_ui/LOGIN/Glues-ESRBRating.png");

	Image* Blizzard_logo = (Image*)App->gui->AdUIElement(470, 475, IMAGE);
	Blizzard_logo->LoadImageA("Homework/wow_ui/MainMenu/Glues-BlizzardLogo.png", 0.66f);


	Button* Manage_Account = (Button*)App->gui->AdUIElement(17, 400, BUTTON);
	Manage_Account->DefineButton("Homework/wow_ui/COMMON/Glue-Panel-Button-Up.png", "Manage Account", "Cuenta", INTERACTABLE);
	Manage_Account->TAB = 1;

	Button* Community_Site = (Button*)App->gui->AdUIElement(17, 439, BUTTON);
	Community_Site->DefineButton("Homework/wow_ui/COMMON/Glue-Panel-Button-Up.png", "Community Site", "Pagina web", INTERACTABLE);
	Community_Site->TAB = 2;

	Button* Login = (Button*)App->gui->AdUIElement(444, 380, BUTTON);
	Login->DefineButton("Homework/wow_ui/COMMON/Glue-Panel-Button-Up.png", "Login", "Iniciar sesion", INTERACTABLE);
	int pos = 350;
	Login->TAB = 5;

	Button* Cinematics = (Button*)App->gui->AdUIElement(872, pos, BUTTON);
	Cinematics->DefineButton("Homework/wow_ui/COMMON/Glue-Panel-Button-Up.png", "Cinematics", "Cinematicas", INTERACTABLE);
	pos += 39;
	Cinematics->TAB = 6;

	Button* Credits = (Button*)App->gui->AdUIElement(872, pos, BUTTON);
	Credits->DefineButton("Homework/wow_ui/COMMON/Glue-Panel-Button-Up.png", "Credits", "Creditos", INTERACTABLE);
	pos += 39;
	Credits->TAB = 7;

	Button* Terms_Of_Use = (Button*)App->gui->AdUIElement(872, pos, BUTTON);
	Terms_Of_Use->DefineButton("Homework/wow_ui/COMMON/Glue-Panel-Button-Up.png", "Terms Of Use", "Terminos de servicio", INTERACTABLE);
	pos = 538 - 20;
	Terms_Of_Use->TAB = 8;

	Button* Quit = (Button*)App->gui->AdUIElement(872, pos, BUTTON);
	Quit->DefineButton("Homework/wow_ui/COMMON/Glue-Panel-Button-Up.png", "Quit", "Salir", INTERACTABLE);
	Quit->TAB = 9;

	//ui_image->rect.add({ 0, 0, 1920, 1080 });

	/*ui_image->LoadImageA("gui/atlas.png");

	ui_image->rect.add({ 642, 169, 229, 69 });
	ui_image->rect.add({ 0,113,229,69 });*/

	text = (Label*)App->gui->AdUIElement(0, 546, LABEL);
	text->SetText("Version 2.0.12 (6546) (Release)");

	text = (Label*)App->gui->AdUIElement(0, 558, LABEL);
	text->SetText("Mar 30 2007");

	text = (Label*)App->gui->AdUIElement(306, 558, LABEL);
	text->SetText("Copyright 2004-2007  Blizzard Entretainment. All Rights Reserved.");

	text = (Label*)App->gui->AdUIElement(464, 264, LABEL);
	text->SetText("Account Name", "Nombre");

	text = (Label*)App->gui->AdUIElement(458, 319, LABEL);
	text->SetText("Account Password", "Contraseña");


	Button* Name = (Button*)App->gui->AdUIElement(450, 282, BUTTON);
	Name->DefineButton("Homework/wow_ui/COMMON1/Common-Input-Border.png", "");
	pos = 538 - 20;
	Name->TAB = 3;
	Button* Password = (Button*)App->gui->AdUIElement(450, 335, BUTTON);
	Password->DefineButton("Homework/wow_ui/COMMON1/Common-Input-Border.png", "");
	Name->TAB = 4;

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
			if (tab_button > 9)
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
	
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (StartGame) {
		App->fade->FadeToBlack(this, App->scene, 1.0f);
		StartGame = false;
	}

	return ret;
}

// Called before quitting
bool j1Menu::CleanUp()
{
	LOG("Freeing Menu");


	return true;
}

