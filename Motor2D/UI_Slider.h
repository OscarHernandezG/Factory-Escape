#include "UI.h"
#include "p2SString.h"
#include "p2List.h"
#include "UI_Label.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "SDL\include\SDL.h"


class Slider : public UI_Element
{
private:

public:

	Slider(int x, int y);
	~Slider();

	bool Update(float dt);
	bool CleanUp();

	bool AddImage(Image* newImage);


public:

	bool hovering = false;

	Image* image_bg = nullptr;
	Image* image_butt = nullptr;

	p2List<Image*> Images;

	int x, y;
};

