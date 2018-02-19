#pragma once

#include "UI.h"
#include "UI_Label.h"
#include "UI_Image.h"
#include "p2SString.h"
#include "p2List.h"
#include "SDL\include\SDL.h"

class ImputText : public UI_Element
{
private:

public:

	ImputText(int x, int y);
	~ImputText();

	bool Update(float dt);
	bool CleanUp();

	bool InputText_Actived = false;
	int H, W;

public:

	SDL_TextInputEvent e;

	char* newText = nullptr;
	Label* Label_ImputText;
	Image* Image_imputText;

};



