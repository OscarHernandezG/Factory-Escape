#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "j1Module.h"
#include "Animation.h"
//#include "Enemy_BasicEnemy.h"

#define MAX_ENEMIES 1000


enum ENEMY_TYPES
{
	NO_TYPE,
	BAT,
};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y, path;
};

class j1Enemies : public j1Module
{
public:

	j1Enemies();
	~j1Enemies();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool FreeEnemies();
	void LoadEnemyText();
//	void OnCollision(Collider* c1, Collider* c2);

	bool AddEnemy(ENEMY_TYPES type, int x, int y);

private:

	void SpawnEnemy(const EnemyInfo& info);
	void FindEnemies();
	p2Point<int> spawn;

private:

	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];
	SDL_Texture* sprites;

	float proj_speed = 5.0f;
	float speed_x_mult, speed_y_mult, common_mult, a_mult;

	bool need_target;

};

#endif // __ModuleEnemies_H__