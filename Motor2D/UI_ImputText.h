#pragma once

#include "UI.h"
#include "UI_Label.h"
#include "UI_Image.h"
#include <string.h>
#include "SDL\include\SDL.h"

class ImputText : public UI_Element
{
private:

public:

	ImputText(int x, int y);
	~ImputText();

	bool Update(float dt);
	bool ImputText::Draw(SDL_Texture* sprites);




public:

	bool InputText_Actived = false;
	int H, W;

	std::string text;

	SDL_Texture* texture = nullptr;
	SDL_Rect r;

	Label* labelInputText;
	Image* imageInputText;

};



