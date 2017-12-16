#include "UI_Score.h"
#include "UI.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Gui.h"

Score::Score(int x, int y) : UI_Element(x, y) {
	type = SCORE;

	image = (Image*)App->gui->AdUIElement(x, y, IMAGE);
	label = (Label*)App->gui->AdUIElement(x, y, LABEL);

}


Score::~Score() {

}

bool Score::Update(float dt) {

		

	return true;
}
bool Score::CleanUp() {

//	App->tex->UnLoad(image);

	return true;
}

bool Score::Define(SDL_Rect rect, char* text)
{
	bool ret1 = false, ret2 = false;

	image->image = rect;

	ret2= label->SetText(text);

	if (ret2) {
		this->rect.w = rect.w;
		this->rect.h = rect.h;

		fPoint label_offset = label->original_pos;
		label_offset.x += (rect.w + label->text_info.rect.w / 2);
		label_offset.y += (rect.h - label->text_info.rect.h) / 2;

		label->position = label_offset;
	}
	return ret1 && ret2;
}

