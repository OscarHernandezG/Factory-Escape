#ifndef __ENEMY_DOWN_S_H__
#define __ENEMY_DOWN_S_H__

#include "Enemy.h"

class Bat : public Enemy
{
private:


	Animation Idle;
	Animation Right;
	Animation Left;
	Animation* CurrentAnim = nullptr;

	//SDL_RendererFlip flip = SDL_FLIP_NONE;

	iPoint originalpos;
	int size = 0;

	void LoadAnimation();

	int* animation_x = nullptr;
	int* animation_y = nullptr;
	int* animation_w = nullptr;
	int* animation_h = nullptr;

	p2List<SDL_Rect> animations_list;

public:
	Bat(int x, int y);


//	void OnCollision(Collider* c1, Collider* c2);

	void Move();
	void Draw(SDL_Texture* texture);
};

#endif
