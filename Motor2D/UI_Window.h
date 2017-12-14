
#include "UI.h"
#include "p2SString.h"
#include "p2List.h"
#include "UI_Label.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "SDL\include\SDL.h"


class Window : public UI_Element
{
private:

public:

	Window(int x, int y);
	~Window();

	bool Update(float dt);
	bool CleanUp();

	bool Define(char* path, char* text);

	void SetTextCords(int x, int y);
	void SetTextCords(iPoint pos);
	void SetTextCords(fPoint pos);

	bool AddButton(Button* newButton);
	
public:

	bool hovering = false;

	Image* image = nullptr;
	Label* label = nullptr;
	p2List<Button*> Buttons;

};

