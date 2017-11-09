#ifndef __ENEMY_DOWN_S_H__
#define __ENEMY_DOWN_S_H__

#include "Enemy.h"

class Enemy_Down_S : public Enemy
{
private:


	Animation redGhostUp;
	Animation redGhostDown;
	Animation redGhostLeft;
	Animation redGhostRight;
	iPoint originalpos;
	

public:

	Enemy_Down_S(int x, int y);

	void OnCollision(Collider* c1, Collider* c2);

	void Move();
};

#endif
