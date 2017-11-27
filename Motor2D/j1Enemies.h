#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Textures.h"
//#include "Enemy_BasicEnemy.h"

#define MAX_ENEMIES 20


enum ENEMY_TYPES
{
	NO_TYPE,
	BAT,
	BLOP,
	PLAYER,
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
	void LoadEnemyAnim();

	bool AddEnemy(ENEMY_TYPES type, int x, int y);

	void SpawnEnemy(const EnemyInfo& info);
	void FindEnemies();

	// Load
	//bool Load(pugi::xml_node&  data);
	//{
	//	x = data.child("position").attribute("x").as_int();
	//	y = data.child("position").attribute("y").as_int();

	//	return true;
	//}

	//bool Save(pugi::xml_node& data) const;
	//{
	//	pugi::xml_node player = data.append_child("position");

	//	player.append_attribute("x") = x;
	//	player.append_attribute("y") = y;

	//	return true;
	//}


public:

	p2Point<int> spawn;

	//Animations cords
	int size = 0;
	int* animation_x = nullptr;
	int* animation_y = nullptr;
	int* animation_w = nullptr;
	int* animation_h = nullptr;

	p2List<SDL_Rect> animations_list;

	Enemy* enemies[MAX_ENEMIES];

private:

	EnemyInfo queue[MAX_ENEMIES];
	
	SDL_Texture* sprites;

	float proj_speed = 5.0f;
	float speed_x_mult, speed_y_mult, common_mult, a_mult;

	bool need_target;

};

#endif // __ModuleEnemies_H__