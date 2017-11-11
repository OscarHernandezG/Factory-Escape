#ifndef __ENEMY_DOWN_S_H__
#define __ENEMY_DOWN_S_H__

#include "Enemy.h"

class Bat : public Enemy
{
private:


	Animation Idle;
	Animation Right;
	Animation Left;
	iPoint originalpos;

	void LoadTexture();
	void LoadAnimation();


	int size = 0;

	int* animation_x = nullptr;
	int* animation_y = nullptr;
	int* animation_w = nullptr;
	int* animation_h = nullptr;

	p2List<SDL_Rect> animations_list;

public:

	SDL_Texture* texture;
	Bat(int x, int y);

	void OnCollision(Collider* c1, Collider* c2);

	void Move();
};

#endif
