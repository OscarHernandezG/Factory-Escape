#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"
//#include "j1Entities.h"

struct SDL_Texture;
struct Collider;

enum ENTITY_TYPES;

class Entity
{
protected:
	Animation* animation = nullptr;
	

public:
	fPoint fpos;
	iPoint position;
	SDL_Texture* textureGhost;
	uint hitpoints = 2;
	Collider* collider = nullptr;
	ENTITY_TYPES type;
	bool firstpath = true;

	bool havepath = false;

	iPoint PosToGo = {0,0};

public:
	Entity(int x, int y, ENTITY_TYPES type);
	virtual ~Entity();



	virtual void Move(float dt) {};
	virtual void Draw(SDL_Texture* sprites);

};

#endif // __ENEMY_H__