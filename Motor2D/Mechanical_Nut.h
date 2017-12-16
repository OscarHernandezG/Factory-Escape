#ifndef __MECHANICALNUT_H__
#define __MECHANICALNUT_H__

#include "Entity.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "j1Entities.h"

struct SDL_Rect;

class Mechanical_Nut : public Entity
{
private:

	SDL_Rect Nut_rect;
	double angle = 0;

public:
	Mechanical_Nut(int x, int y, ENTITY_TYPES etype);

	void Move(float dt);
	void Draw(SDL_Texture* texture);
};

#endif
