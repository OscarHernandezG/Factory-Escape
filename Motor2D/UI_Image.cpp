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

	if (draw)
		App->render->Blit(image, position.x, position.y, &rect, 0);

	return true;
}
bool Image::CleanUp() {

	App->tex->UnLoad(image);	
	
	return true;
}
bool Image::LoadUI_Image(char* path, float scale) {

	image=App->tex->Load(path);

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