#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "Module.h"
#include "Enemy_BasicEnemy.h"

#define MAX_ENEMIES 1000


enum ENEMY_TYPES
{
	NO_TYPE,
	BASICENEMY,
	BASICENEMY2,
	GREENSHIP,
	POWERUP_SHIP,
	POWERUP,
	TURRET,
	BROWN_TANK,
	GREY_TANK,
	BOAT,
	BIG_BOAT,
	BEE,
	BOSS,
	BIGBROWNTANK,
	BIGGREYTANK,
	BOX,
	DARKBOX,
	EXTRAPOINTS,
	MISSILE,
	WAGON,
	BOMB
};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y, path;
};

class ModuleEnemies : public Module
{
public:

	ModuleEnemies();
	~ModuleEnemies();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	bool FreeEnemies();
	void OnCollision(Collider* c1, Collider* c2);

	bool AddEnemy(ENEMY_TYPES type, int x, int y, int path = 0);

private:

	void SpawnEnemy(const EnemyInfo& info);

private:

	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];
	SDL_Texture* sprites;

	float proj_speed = 5.0f;
	float speed_x_mult, speed_y_mult, common_mult, a_mult;

	bool need_target;

};

#endif // __ModuleEnemies_H__