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
#include "j1Menu.h"


Button::Button(int x, int y, bool HUD) : UI_Element(x, y) {

	if (HUD) {
		image = (Image*)App->gui->AdHUDElement(x, y, IMAGE);
		MouseClick = (Image*)App->gui->AdHUDElement(x, y, IMAGE);
		MouseHovering = (Image*)App->gui->AdHUDElement(x, y, IMAGE);
	}
	else {
		image = (Image*)App->gui->AdUIElement(x, y, IMAGE);
		MouseClick = (Image*)App->gui->AdUIElement(x, y, IMAGE);
		MouseHovering = (Image*)App->gui->AdUIElement(x, y, IMAGE);

	}
	label = (Label*)App->gui->AdUIElement(x, y, LABEL);

	MouseHovering->draw = MouseClick->draw = false;
	type = BUTTON;
}

bool Button::Update(float dt)
{
	MouseHovering->draw = MouseClick->draw = false;
	image->position = label->original_pos = MouseClick->position = MouseHovering->position = this->position;
	bool ret = true;


	if (App->menu->debug) {
		debug_UI.x = position.x;
		debug_UI.y = position.y;
		App->render->DrawQuad(debug_UI, 255, 0, 0, 255, false);
	}
	if (MouseOnRect()) {
		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT) {
			MouseClick->draw = true;
		}
		else if (App->input->GetMouseButtonDown(1) == KEY_UP) {
			for (p2List_item<j1Module*>* iterator = listeners.start; iterator != nullptr; iterator = iterator->next)
				iterator->data->GUICallback(this);
		}
		else
			MouseHovering->draw = true;
	}

	if (TAB == App->menu->tab_button) {

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT)
			MouseClick->draw = true;
		else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP) {
			for (p2List_item<j1Module*>* iterator = listeners.start; iterator != nullptr; iterator = iterator->next)
				iterator->data->GUICallback(this);
		}
		else
			MouseHovering->draw = true;
	}


	fPoint label_offset = label->original_pos;
	label_offset.x += (rect.w - label->text_info.rect.w) / 2;
	label_offset.y += (rect.h - label->text_info.rect.h) / 2;

	label->position = label_offset;

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

//	MouseClick->CleanUp();
//	label->CleanUp();
//	image->CleanUp();
//	MouseHovering->CleanUp();

	listeners.clear();

	return true;
}


bool Button::Define(SDL_Rect idle, SDL_Rect hovering, SDL_Rect oncick, char* text)
{
	bool ret1 = false, ret2 = false, ret3 = false, ret4 = false;

	ret1 = MouseHovering->LoadUI_Image(hovering, 1);
	ret2 = MouseClick->LoadUI_Image(oncick, 1);

	ret3 = image->LoadUI_Image(idle, 1);

	ret4 = label->SetText(text);

	rect.w = debug_UI.w = image->image.w;
	rect.h = debug_UI.h = image->image.h;

	if (ret4) {

		fPoint label_offset = label->original_pos;
		label_offset.x += (rect.w - label->text_info.rect.w) / 2;
		label_offset.y += (rect.h - label->text_info.rect.h) / 2;

		label->position = label_offset;
		
	}
	return ret1 && ret2 && ret3 && ret4;
}

void Button::AddListener(j1Module* listener) {
	listeners.add(listener);
}

void Button::AddListener(p2List<j1Module*> listener) {
	for (p2List_item<j1Module*>* iterator = listener.start; iterator != nullptr; iterator = iterator->next) {
		listeners.add(iterator->data);
	}
}
