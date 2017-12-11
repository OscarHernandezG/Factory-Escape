#include "UI_Image.h"
#include "UI.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"

Image::Image(int x, int y) : UI_Element(x, y) {
	type = IMAGE;
}


Image::~Image() {

}

bool Image::Update(float dt) {

	App->render->Blit(image.start->data, position.x, position.y, &rect, 0);

	return true;
}
bool Image::CleanUp() {

	return true;
}
bool Image::LoadImageA(char* path, float scale) {

	image.add(App->tex->Load(path));

	position.x /= scale;
	position.y /= scale;
	imagen.create(path);
	uint w = 0, h = 0;
	App->tex->GetSize(image.end->data, w, h);
	int rect_w = w;
	int rect_h = h;
	rect = { 0,0,rect_w,rect_h };

	this->scale = scale;

	return true;
}

bool Image::MouseOnRect() {
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

bool Image::MouseClick() {
	bool ret = false;

	if (MouseOnRect()) {
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN) {

		}
	}

	return ret;
}