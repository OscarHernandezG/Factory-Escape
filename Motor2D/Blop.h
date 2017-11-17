#ifndef __BLOP_H__
#define __BLOP_H__

#include "Enemy.h"
#include "p2Point.h"
#include "p2DynArray.h"

class Blop : public Enemy
{
private:

	Animation Idle;
	Animation Walk;
	Animation* CurrentAnim = nullptr;

	//SDL_RendererFlip flip = SDL_FLIP_NONE;

	iPoint originalpos;

	void LoadAnimation();

public:
	Blop(int x, int y);

	p2DynArray<iPoint>* path;

	//	void OnCollision(Collider* c1, Collider* c2);

	void Move(float dt);
	void Draw(SDL_Texture* texture);
};

#endif
