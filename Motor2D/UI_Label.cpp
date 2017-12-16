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

	//App->tex->UnLoad(text_info.tex);

	return true;
}
bool Label::SetText(char* text, int font) {

	bool ret = false;
	p2SString aux_text;
	aux_text.create(text);

	if (text_info.tex != nullptr)
	App->tex->UnLoad(text_info.tex);
	SDL_Texture* tex;
	if (font == 0)
		 tex = App->font->Print(aux_text.GetString(), { 255, 255, 255, 255 }, App->font->fonts.start->data);
	else 
		 tex = App->font->Print(aux_text.GetString(), { 255, 255, 255, 255 }, App->font->fonts.end->data);
	if (tex != nullptr) {
		uint w = 0, h = 0;
		App->tex->GetSize(tex, w, h);
		int rect_w = w;
		int rect_h = h;
		SDL_Rect rect = { 0,0,rect_w,rect_h };

		text_info = { tex, rect, aux_text };
		ret = true;
	}
	return ret;
}
