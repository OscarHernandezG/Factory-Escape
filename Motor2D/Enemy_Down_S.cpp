#include "j1App.h"
#include "Enemy_Down_S.h"
#include "j1Player.h"


Enemy_Down_S::Enemy_Down_S(int x, int y) : Enemy(x, y)
{

	originalpos.x = x;
	originalpos.y = y;




}

void Enemy_Down_S::Move()
{

}


void Enemy_Down_S::OnCollision(Collider* c1, Collider* c2) {

}