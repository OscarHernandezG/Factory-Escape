#include "UI_ImputText.h"
#include "UI.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "UI_Button.h"
#include "j1Menu.h"
#include "j1Input.h"

ImputText::ImputText(int x, int y) : UI_Element(x, y) {

	type = IMPUT_TEXT;
	original_pos = position;

	imageInputText = (Image*)App->gui->AdUIElement(x, y, IMAGE);
	labelInputText = (Label*)App->gui->AdUIElement(x, y, LABEL);


}

ImputText::~ImputText() {

}


bool ImputText::Update(float dt) {
	
	if (InputText_Actived) {	
		App->render->DrawQuad({ 1000 + r.w,10,3,13 }, 255, 255, 255, 255);
		if (App->input->isPresed) {
			text += App->input->newLetter;
			App->font->CalcSize(text.data(), r.w, r.h);
			texture = App->font->Print(text.data());
			App->input->isPresed = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) {
			text.pop_back();
			App->font->CalcSize(text.data(), r.w, r.h);
			texture = App->font->Print(text.data());
		}
		App->render->Blit(texture, 1000, 10, &r);
	}



	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		InputText_Actived = !InputText_Actived;
		if (InputText_Actived) {
			labelInputText->SetText("");
			SDL_StartTextInput();

		}
		else if (!InputText_Actived) {
			labelInputText->SetText("Your Name");
			SDL_StopTextInput();
		}
	}

	return true;
}
bool ImputText::Draw(SDL_Texture* sprites) {

	return true;
}