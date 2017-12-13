#include "UI_Button.h"
#include "UI.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Menu.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Gui.h"


Button::Button(int x, int y) : UI_Element(x, y) {


	image = (Image*)App->gui->AdUIElement(x, y, IMAGE);
	label = (Label*)App->gui->AdUIElement(x, y, LABEL);
	type = BUTTON;
	MouseClick = (Image*)App->gui->AdUIElement(x + 6, y + 8, IMAGE);
	MouseHovering = (Image*)App->gui->AdUIElement(x, y, IMAGE);
	
	MouseHovering->draw = MouseClick->draw = false;

	// Idle.PushBack(); ...
	// Clicked.PushBack(); ...
	// MouseOnRect.PushBack(); ...
	//CurrAnim = Idle;
}


bool Button::Update(float dt) 
{
	bool ret = true;
	MouseHovering->draw = MouseClick->draw = false;
	if (Button_type == INTERACTABLE) {
		if (MouseOnRect()) {
			if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
				MouseClick->draw = true;
			else if (App->input->GetMouseButtonDown(1) == KEY_UP) {			
				for (p2List_item<j1Module*>* iterator = listeners.start; iterator != nullptr; iterator = iterator->next) 
					iterator->data->GUICallback(this);
			}
			
				MouseHovering->draw = true;
				//App->render->Blit(MouseClick, position.x + 20, position.y + 8);
			
		}

		if (TAB == App->menu->tab_button) {
		/*	if (SDL_SCANCODE_RETURN == KEY_REPEAT)
				MouseClick->draw = true;
			else if (SDL_SCANCODE_RETURN == KEY_UP) {
				for (p2List_item<j1Module*>* iterator = listeners.start; iterator != nullptr; iterator = iterator->next)
					iterator->data->GUICallback(this);
			}*/
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT) 
				MouseClick->draw = true;
			else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP) {
				for (p2List_item<j1Module*>* iterator = listeners.start; iterator != nullptr; iterator = iterator->next)
					iterator->data->GUICallback(this);
			}
			else
				MouseHovering->draw = true;
		}
	}


	if (label->hovering) {
		iPoint label_offset = label->original_pos;
		label_offset.x += (rect.w - label->text_info.end->data.rect.w) / 2;
		label_offset.y += (rect.h - label->text_info.end->data.rect.h) / 2;

		label->position = label_offset;
	}
	else
	{
		iPoint label_offset = label->original_pos;
		label_offset.x += (rect.w - label->text_info.start->data.rect.w) / 2;
		label_offset.y += (rect.h - label->text_info.start->data.rect.h) / 2;

		label->position = label_offset;
	}
	//App->render->Blit(image->image.start->data, position.x, position.y/*,CurrAnim->GetCurrentFrame()*/);
	//CurrAnim = Idle;

//	MouseClick->Update(dt);

	return true;
}

bool Button::Clicked() {
	/*
	CurrAnim = Clicked;
	*/
	//clicked = true;
	return true;
}

bool Button::MouseOnRect() {
	bool ret = false;

	iPoint mouse{ 0,0 };
	App->input->GetMousePosition(mouse.x, mouse.y);

	if (mouse.x > position.x && mouse.x < position.x + rect.w) {
		if (mouse.y > position.y && mouse.y < position.y + rect.h) {
			ret = true;
		}
	}

	return ret;
}

bool Button::CleanUp() {

	MouseClick->CleanUp();
	label->CleanUp();
	image->CleanUp();
	MouseHovering->CleanUp();
	listeners.clear();

	return true;
}


void Button::DefineButton(char* path, char* text, char* textesp, BUTTON_TYPE type)
{
	
	MouseHovering->LoadImageA("textures/Hover_But.png",1);
	MouseClick->LoadImageA("textures/Click_But.png",1);

	image->LoadImageA(path, 1);

	label->SetText(text, textesp);
	rect.w = image->rect.w;
	rect.h = image->rect.h;

	iPoint label_offset{ 0,0 };
	//label_offset.x = (rect.w - label->text_info.start->data.rect.w) / 2;
	//label_offset.y = (rect.h - label->text_info.start->data.rect.h) / 2;

	//label->position += label_offset;

	this->Button_type = type;
}

void Button::AddListener(j1Module* listener) {
	listeners.add(listener);
}

void Button::AddListener(p2List<j1Module*> listener) {
	for (p2List_item<j1Module*>* iterator = listener.start; iterator != nullptr; iterator = iterator->next) {
		listeners.add(iterator->data);
	}
}
