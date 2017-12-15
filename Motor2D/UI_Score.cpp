#include "UI_Score.h"
#include "UI.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"

Score::Score(int x, int y) : UI_Element(x, y) {
	type = IMAGE;
}


Score::~Score() {

}

bool Score::Update(float dt) {

	if (draw)
		App->render->Blit(image, position.x, position.y, &rect, 0);

	return true;
}
bool Score::CleanUp() {

	App->tex->UnLoad(image);

	return true;
}
bool Score::LoadImageA(char* path, float scale) {

	image = App->tex->Load(path);

	position.x /= scale;
	position.y /= scale;
	imagen.create(path);
	uint w = 0, h = 0;
	App->tex->GetSize(image, w, h);
	int rect_w = w;
	int rect_h = h;
	rect = { 0,0,rect_w,rect_h };

	this->scale = scale;

	return image != nullptr;
}

bool Score::MouseOnRect() {
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

bool Score::MouseClick() {
	bool ret = false;

	if (MouseOnRect()) {
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN) {

		}
	}

	return ret;
}