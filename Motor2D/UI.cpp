#include "UI.h"
#include "j1Gui.h"
#include "j1App.h"



UI_Element::UI_Element(int x, int y) : position(x,y) 
{
	atlas = App->gui->GetAtlas();
}

// Called each loop iteration
bool UI_Element::Update(float dt) {
	
	return true;
}

// Called each loop iteration
bool UI_Element::Draw(SDL_Texture* sprites) {
	return true;
}

// Called before quitting
bool UI_Element::CleanUp() {
	return true;
}

bool UI_Element::MouseOnRect() {
	return true;
}



