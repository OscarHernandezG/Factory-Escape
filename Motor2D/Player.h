#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Animation.h"
#include "j1Render.h"
#include "Entity.h"
#include "p2Point.h"
#include "p2DynArray.h"


struct SDL_Texture;
struct SDL_Rect;


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
	MELEE_JUMP_RIGHT,
	MELEE_JUMP_LEFT,
	SHOOT_RIGHT,
	SHOOT_LEFT,

	PLAYER_WIN,

	DEAD

};


class Player : public Entity
{
public:
	Player(int x, int y);

	void Move(float dt) {
	
	}
	void Draw(SDL_Texture* texture) {}



	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load
	bool Load(pugi::xml_node&  savegame);

	//Save
	bool Save(pugi::xml_node& data) const;

	//void LoadTexture();

	void CheckPlayerState(float dt);

	void LoadAnimations();

	void FindSpawn();

	void SpawnPlayer();

	void KillEnemies();

	bool CheckPlayerDeath();


	float x = 0.0f, y = 0.0f;
	bool god_mode = false;

private:


	SDL_Texture* texture = nullptr;
	SDL_Rect rect;
	float angle = 0;
	p2Point<float> speed;
	p2Point<float> movement = { 0,0 };
	float a = 0.002;

	bool run_left = false;
	bool run_right = false;
	bool jump = false;
	bool CanJump = true;
	bool slide = false;
	bool ground = false;
	bool death = false;
	bool melee = false;

	State PlayerState = IDLE;

	int down_force = 0;
	int jump_force = 2000;


	Animation Idle;
	Animation Jump;
	Animation Run;
	Animation Slide;
	Animation Die;
	Animation Melee;
	Animation MeleeJump;
	Animation Shoot;
	Animation* CurrentAnim = nullptr;
	uint pos = 0, lastTime = 0, currentTime = 0, dieTime = 0;

	int size = 0;

	int* animation_x = nullptr;
	int* animation_y = nullptr;
	int* animation_w = nullptr;
	int* animation_h = nullptr;

	p2List<SDL_Rect> animations_list;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	p2Point<int> spawn;
};



#endif // !__PLAYER_H__


