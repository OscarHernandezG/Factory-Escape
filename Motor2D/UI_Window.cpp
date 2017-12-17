#include "UI_Window.h"
#include "UI.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "UI_Button.h"
#include "j1Menu.h"

Window::Window(int x, int y) : UI_Element(x, y) {

	type = WINDOW;
	original_pos = position;

	image = (Image*)App->gui->AdUIElement(x, y, IMAGE);
	label = (Label*)App->gui->AdUIElement(x, y, LABEL);

}

Window::~Window() {

}


bool Window::Update(float dt) {
	if (App->menu->debug) {
		debug_UI.x = position.x;
		debug_UI.y = position.y;
		App->render->DrawQuad(debug_UI, 255, 0, 0, 255, false);
	}
	return true;
}
bool Window::CleanUp() {


	return true;
}

bool Window::Define(SDL_Rect rect, char* text) {

	bool ret1 = false, ret2 = false;


	ret1 = image->LoadUI_Image(rect, 1);

	ret2 = label->SetText(text);

	debug_UI = rect;
	
	return ret1 && ret2;
}

void Window::SetTextCords(int x, int y) {

	label->position.x = x;
	label->position.y = y;
}

void Window::SetTextCords(iPoint pos) {

	label->position.x = pos.x;
	label->position.y = pos.y;
}
void Window::SetTextCords(fPoint pos) {

	label->position = pos;
}

bool Window::AddButton(Button* newButton) {
	if (newButton != nullptr) {
		Buttons.add(newButton);
		return true;
	}
	else return false;
}
