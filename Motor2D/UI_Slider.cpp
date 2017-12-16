#include "UI_Slider.h"
#include "UI.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "UI_Button.h"


Slider::Slider(int x, int y) : UI_Element(x, y) {

	type = SLIDER;
	original_pos = position;

	image_bg = (Image*)App->gui->AdUIElement(x, y, IMAGE);
	image_butt = (Image*)App->gui->AdUIElement(x, y, IMAGE);

}

Slider::~Slider() {
}


bool Slider::Update(float dt) {
	int x = 0, y = 0;
	if (!moving_mouse) {
		moving_mouse = Clicked();
	}

	else if (moving_mouse) {
		App->input->GetMouseMotion(x, y);
		if (image_butt->position.x + x >= image_bg->position.x && image_butt->position.x + x + image_butt->image.w <= image_bg->position.x + image_bg->image.w) {
			image_butt->position.x += x;
			for (p2List_item<j1Module*>* iterator = listeners.start; iterator != nullptr; iterator = iterator->next)
				iterator->data->GUICallback(this);
		}
		if (App->input->GetMouseButtonDown(1) == KEY_UP) {
			moving_mouse = false;
		}
	}
	return true;
}
bool Slider::CleanUp() {

	listeners.clear();

	return true;
}


bool Slider::MouseOnRect() {
	bool ret = false;

	iPoint mouse{ 0,0 };
	App->input->GetMousePosition(mouse.x, mouse.y);

	if (mouse.x > image_butt->position.x && mouse.x < image_butt->position.x + image_butt->image.w) {
		if (mouse.y > image_butt->position.y && mouse.y < image_butt->position.y + image_butt->image.h) {
			ret = true;
		}
	}

	return ret;
}

bool Slider::Clicked() {
	bool ret = false;
	if (MouseOnRect()) {
		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT) {
			ret = true;
		}
	}
	return ret;
}

float Slider::GetRelativePosition() {
	float ret = 0;

	int relative_x = image_butt->position.x - image_bg->position.x;

	ret = (float)relative_x / (image_bg->image.w - image_butt->image.w);

	return ret;
}

void Slider::AddListener(j1Module* listener) {
	listeners.add(listener);
}

void Slider::SetRelativePos(float x) {

	float new_x = x + (image_bg->position.x / (image_bg->image.w - image_butt->image.w));
	new_x *= (image_bg->image.w - image_butt->image.w);

	image_butt->position.x = new_x;

}

bool Slider::Define(SDL_Rect bg, SDL_Rect butt) {
	bool ret1 = true, ret2 = true;

	
	ret1 = image_bg->LoadUI_Image(bg);
	ret2 = image_butt->LoadUI_Image(butt);

	return ret1 && ret2;
}
