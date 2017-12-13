#ifndef __ModuleEntities_H__
#define __ModuleEntities_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Textures.h"
#include "Player.h"
#include "Entity.h"

#define MAX_ENEMIES 20

class Player;

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
	bool Load(pugi::xml_node&  savegame);
	bool Save(pugi::xml_node& data) const;
	bool FreeEnemies();
	void LoadEntityText();
	void LoadEntityAnim();

	bool AddEntity(ENTITY_TYPES type, int x, int y);

	void SpawnEnemy(const EntityInfo& info);


public:

	p2Point<int> spawn;

	//Animations cords
	int size = 0;
	int* animation_x = nullptr;
	int* animation_y = nullptr;
	int* animation_w = nullptr;
	int* animation_h = nullptr;

	p2List<SDL_Rect> animations_list;

	p2List<Entity*> entities;
	Player* player = nullptr;

	SDL_Texture* sprites = nullptr;
	SDL_Texture* Player_Sprites = nullptr;
private:

	EntityInfo queue[MAX_ENEMIES];
	
	//SDL_Texture* sprites = nullptr;
	//SDL_Texture* Player_Sprites = nullptr;

	float proj_speed = 5.0f;
	float speed_x_mult, speed_y_mult, common_mult, a_mult = 0.0f;

};

#endif // __ModuleEnemies_H__