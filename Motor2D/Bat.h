#ifndef __BAT_H__
#define __BAT_H__

#include "Enemy.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"

class Bat : public Enemy
{
private:

	int x, y;
	Animation Idle;
	Animation Right;
	Animation Left;
	Animation* CurrentAnim = nullptr;

	//SDL_RendererFlip flip = SDL_FLIP_NONE;

	iPoint originalpos;

	void LoadAnimation();
public:
	Bat(int x, int y);

	p2DynArray<iPoint>* path;

//	void OnCollision(Collider* c1, Collider* c2);
	j1Timer pf;
	void Move(float dt);
	void Draw(SDL_Texture* texture);
};

#endif
