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
	image_butt = (Image*)App->gui->AdUIElement(x, y, LABEL);

}

Slider::~Slider() {
}


bool Slider::Update(float dt) {

	App->input->GetMouseMotion(x, y);

	return true;
}
bool Slider::CleanUp() {

	return true;
}


bool Slider::AddImage(Image* newImage) {
	if (newImage != nullptr) {
		Images.add(newImage);
		return true;
	}
	else return false;
}