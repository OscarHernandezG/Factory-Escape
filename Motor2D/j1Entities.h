#ifndef __ModuleEntities_H__
#define __ModuleEntities_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Textures.h"
#include "Player.h"
//#include "Enemy_BasicEnemy.h"

#define MAX_ENEMIES 20


enum ENTITY_TYPES
{
	NO_TYPE,
	BAT,
	BLOP,
	PLAYER,
};

class Entity;

struct EntityInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::NO_TYPE;
	int x, y, path;
};

class j1Entities : public j1Module
{
public:

	j1Entities();
	~j1Entities();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool FreeEnemies();
	void LoadEntityText();
	void LoadEntityAnim();

	bool AddEntity(ENTITY_TYPES type, int x, int y);

	void SpawnEnemy(const EntityInfo& info);

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

	Entity* entities[MAX_ENEMIES];
	Player* player = nullptr;
private:

	EntityInfo queue[MAX_ENEMIES];
	
	SDL_Texture* sprites = nullptr;
	SDL_Texture* Player_Sprites = nullptr;

	float proj_speed = 5.0f;
	float speed_x_mult, speed_y_mult, common_mult, a_mult = 0.0f;

};

#endif // __ModuleEnemies_H__