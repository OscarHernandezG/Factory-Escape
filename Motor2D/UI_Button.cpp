#include "UI_Button.h"
#include "UI.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1FadeToBlack.h"


Button::Button(int x, int y) : UI_Element(x, y) {
	image = (Image*)App->gui->AdUIElement(x, y, IMAGE);
	label = (Label*)App->gui->AdUIElement(x, y, LABEL);
	type = BUTTON;

	MouseHovering = App->tex->Load("Homework/wow_ui/COMMON/Glue-Panel-Button-Glow.png"); 
	MouseClick = App->tex->Load("Homework/wow_ui/BUTTONS/UI-DialogBox-Button-Down.png");

	// Idle.PushBack(); ...
	// Clicked.PushBack(); ...
	// MouseOnRect.PushBack(); ...
	//CurrAnim = Idle;
}


bool Button::Update(float dt) 
{
	if (Button_type == INTERACTABLE) {
		if (MouseOnRect()||TAB == App->scene->tab_button)
			if(App->input->GetMouseButtonDown(1) == KEY_REPEAT)
				App->render->Blit(MouseClick, position.x+5, position.y+8);
			else
				App->render->Blit(MouseHovering, position.x - 3, position.y + 1);
	}
	if (App->input->GetMouseButtonDown(1) == KEY_DOWN && TAB == 5 && !App->fade->IsFading())
		App->scene->StartGame = true;

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
	App->render->Blit(image->image.start->data, position.x, position.y/*,CurrAnim->GetCurrentFrame()*/);
	//CurrAnim = Idle;
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
	return true;
}


void Button::DefineButton(char* path, char* text, char* textesp, BUTTON_TYPE type)
{
	image->LoadImageA(path,1);
	label->SetText(text, textesp);
	rect.w = image->rect.w;
	rect.h = image->rect.h;

	iPoint label_offset{ 0,0 };
	//label_offset.x = (rect.w - label->text_info.start->data.rect.w) / 2;
	//label_offset.y = (rect.h - label->text_info.start->data.rect.h) / 2;

	//label->position += label_offset;

	this->Button_type = type;
}