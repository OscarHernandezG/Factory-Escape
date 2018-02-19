#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_Image.h"
#include "UI_Label.h"
#include "UI_Button.h"
#include "UI_Window.h"
#include "UI_Slider.h"
#include "UI_Score.h"
#include "UI_ImputText.h"
#include "j1Audio.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");
	
	button_idle = { 0,0,0,0 };
	button_hovering = { 0,0,0,0 };
	button_onclick = { 0,0,0,0 };

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	button_click_fx = App->audio->LoadFx("audio/fx/button_click.wav");


	button_idle = { 41, 33, 244, 77 };
	button_hovering = { 318, 33, 244, 77 };
	button_onclick = { 609, 33, 244, 77 };
	
	
	

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt)
{
///
	if (atlas == nullptr)
		App->tex->Load(atlas_file_name.GetString());


	for (p2List_item<UI_Element*>* iterator = ui_elements.start; iterator != nullptr; iterator = iterator->next) {
			iterator->data->Update(dt);
	}

	for (p2List_item<UI_Element*>* iterator = hud_elements.start; iterator != nullptr; iterator = iterator->next) {
		iterator->data->Update(dt);
	}


	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	for (p2List_item<UI_Element*>* iterator = App->gui->ui_elements.start; iterator != nullptr; iterator = iterator->next)
		iterator->data->CleanUp();

	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

UI_Element* j1Gui::AdUIElement(int x,int y, GUI_TYPE type)
{
	UI_Element* ret = nullptr;

	switch (type)
	{
	case NO_TYPE_GUI:
		break;
	case IMAGE:
		ret = new Image(x, y);
		break;
	case LABEL:
		ret = new Label(x, y);
		break;
	case BUTTON:
		ret = new Button(x, y,false);
		break;
	case WINDOW:
		ret = new Window(x, y);
		break;
	case SLIDER:
		ret = new Slider(x, y);
		break;
	case SCORE:
		ret = new Score(x, y);
		break;
	case IMPUT_TEXT:
		ret = new ImputText(x, y);
		break;
	default:
		break;
	}

	ui_elements.add(ret);


	return ret;
}


UI_Element* j1Gui::AdHUDElement(int x, int y, GUI_TYPE type)
{
	UI_Element* ret = nullptr;

	switch (type)
	{
	case NO_TYPE_GUI:
		break;
	case IMAGE:
		ret = new Image(x, y);
		break;
	case LABEL:
		ret = new Label(x, y);
		break;
	case BUTTON:
		ret = new Button(x, y, true);
		break;
	case WINDOW:
		ret = new Window(x, y);
		break;
	case SLIDER:
		ret = new Slider(x, y);
		break;
	case SCORE:
		ret = new Score(x, y);
		break;
	case IMPUT_TEXT:
		ret = new ImputText(x, y);
		break;
	default:
		break;
	}

	hud_elements.add(ret);


	return ret;
}

// class Gui ---------------------------------------------------

