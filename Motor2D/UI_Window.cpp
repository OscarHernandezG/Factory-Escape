#include "UI_Window.h"
#include "UI.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "UI_Button.h"

Window::Window(int x, int y) : UI_Element(x, y) {

	type = WINDOW;
	original_pos = position;

	image = (Image*)App->gui->AdUIElement(x, y, IMAGE);
	label = (Label*)App->gui->AdUIElement(x, y, LABEL);

}

Window::~Window() {

}


bool Window::Update(float dt) {



	return true;
}
bool Window::CleanUp() {

	return true;
}

bool Window::Define(char* path, char* text) {

	bool ret1 = false, ret2 = false;


	ret1 = image->LoadUI_Image(path, 1);

	ret2 = label->SetText(text);

	
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
