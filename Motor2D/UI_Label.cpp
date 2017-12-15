#include "UI_Label.h"
#include "UI.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"

Label::Label(int x, int y) : UI_Element( x,  y) {

	type = LABEL;
	original_pos = position;

}

Label::~Label() {

}


bool Label::Update(float dt) {

	App->render->Blit(text_info.tex, position.x, position.y, &text_info.rect, 0);

	return true;
}
bool Label::CleanUp() {
//	this->text.Clear();
	//for (p2List_item<TextInfo>* iterator = text_info.start; iterator != nullptr; iterator = iterator->next) {
	//	delete iterator;
	//}
	//text_info.clear();
	return true;
}
bool Label::SetText(char* text) {
	
	p2SString aux_text;
	aux_text.create(text);
	SDL_Texture* tex = App->font->Print(aux_text.GetString(), { 255, 255, 255, 255 }, App->font->fonts.start->data);
	uint w = 0, h = 0;
	App->tex->GetSize(tex, w, h);
	int rect_w = w;
	int rect_h = h;
	SDL_Rect rect = { 0,0,rect_w,rect_h };

	text_info = { tex, rect, aux_text };

	return true;
}
