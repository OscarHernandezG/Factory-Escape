#include "j1App.h"
#include "Enemy.h"
#include "j1Render.h"

Enemy::Enemy(int x, int y) : position(x, y)
{
	

	

}

Enemy::~Enemy()
{

}

void Enemy::Draw(SDL_Texture* sprites)
{
	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
}

