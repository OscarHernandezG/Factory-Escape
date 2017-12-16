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
		App->render->Blit(atlas, position.x, position.y, &image, 0);

	return true;
}
bool Image::CleanUp() {
	
	return true;
}
bool Image::LoadUI_Image(SDL_Rect image_rect, float scale) {

	image = image_rect;

	position.x /= scale;
	position.y /= scale;

	

	this->scale = scale;

	return true;
}