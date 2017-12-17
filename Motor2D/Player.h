#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Animation.h"
#include "j1Render.h"
#include "Entity.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Entities.h"

struct SDL_Texture;
struct SDL_Rect;

enum Player_collision {
	NON_COLLISION,
	ENEMY_COLLISION,
	ITEM_COLLISION
};


enum State {
	IDLE = 0,
	RUNNING_RIGHT,
	RUNNING_LEFT,
	JUMPING_RIGHT,
	JUMPING_LEFT,
	SHIFT_RIGHT,
	SHIFT_LEFT,

	MELEE,
	MELEE_RIGHT,
	MELEE_LEFT,

	PLAYER_WIN,

	DEAD

};


class Player : public Entity
{
public:
	Player(int x, int y, ENTITY_TYPES type);

	void Move(float dt);
	void Draw(SDL_Texture* texture);



	// Destructor
	virtual ~Player();

	void CheckPlayerState(float dt);

	void LoadAnimations();

	bool FindSpawn();

	void SpawnPlayer();

	void KillEnemies();

	Player_collision CheckPlayerCollision();


public:

	bool god_mode = false;

private:


	SDL_Texture* texture = nullptr;
	SDL_Rect rect;
	fPoint speed = { 0.0f,0.0f };

	bool run_left = false;
	bool run_right = false;
	bool jump = false;
	bool CanJump = true;
	bool slide = false;
	bool ground = false;
	bool death = false;
	bool melee = false;

	Player_collision checkColl;

	State PlayerState = IDLE;

	int down_force = 0;
	int jump_force = 2000;


	Animation Idle;
	Animation Jump;
	Animation Run;
	Animation Slide;
	Animation Die;
	Animation Melee;
	Animation* CurrentAnim = nullptr;
	uint pos = 0, lastTime = 0, currentTime = 0, dieTime = 0;

	int size = 0;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	iPoint spawn;
};



#endif // !__PLAYER_H__


