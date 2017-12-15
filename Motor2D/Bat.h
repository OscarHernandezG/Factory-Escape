#ifndef __BAT_H__
#define __BAT_H__

#include "Entity.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "j1Entities.h"

class Bat : public Entity
{
private:

	int x;
	Animation Idle;
	Animation Right;
	Animation Left;
	Animation* CurrentAnim = nullptr;

	//SDL_RendererFlip flip = SDL_FLIP_NONE;

	void LoadAnimation();
public:
	Bat(int x, int y, ENTITY_TYPES etype);

	p2DynArray<iPoint>* path;
	int speed = 150;


	j1Timer pf;
	void Move(float dt);
	void Draw(SDL_Texture* texture);
};

#endif
