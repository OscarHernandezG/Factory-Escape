#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Enemies.h"
#include "Bat.h"



#define SPAWN_MARGIN 50

j1Enemies::j1Enemies()
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

// Destructor
j1Enemies::~j1Enemies()
{
}

bool j1Enemies::Start()
{
	if (sprites == nullptr) {
		LOG("No cargado");
	}
	sprites = App->tex->Load("assets/textures/enemieslvl2.png");
	if (sprites != nullptr) {
		LOG("Cargado");
	}

	FindEnemies();

	return true;
}

bool j1Enemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			if (-queue[i].y < (App->render->camera.y) + SPAWN_MARGIN / 2)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].x);

			}

		}
	}



	return true;
}

// Called before render is available
bool j1Enemies::Update()
{

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Draw(sprites);

	return true;
}

bool j1Enemies::PostUpdate()
{
	// check camera position to decide what to spawn
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			if (-enemies[i]->position.y/*screen_Size*/ < (App->render->camera.y) - SPAWN_MARGIN * 16)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.x /* * screen_Size*/);
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}

	return true;
}

// Called before quitting
bool j1Enemies::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(sprites);

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
		queue[i].type = NO_TYPE;
	}

	return true;
}

bool j1Enemies::FreeEnemies()
{

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
		queue[i].type = NO_TYPE;
	}

	return true;
}

bool j1Enemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Enemies::SpawnEnemy(const EnemyInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for(; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENEMY_TYPES::BAT:
			enemies[i] = new Bat(info.x, info.y);
			break;
		}			
	}
}

void j1Enemies::FindEnemies()
{
	p2List_item<MapLayer*>* layer = App->map->data.layers.end;
	for (int i = 0; i < layer->data->size_data; i++)
	{
		if (layer->data->data[i] == Tile_Type::BAT_SPAWN)
		{
			iPoint spawn = App->map->TileToWorld(i);
			AddEnemy(ENEMY_TYPES::BAT, spawn.x,spawn.y);
		}
	}

}

/*void j1Enemies::OnCollision(Collider* c1, Collider* c2)
{

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{

		if (enemies[i] != nullptr) {
			if (enemies[i]->GetCollider() == c1 && enemies[i]->hittable) {

				c1->life_units--;

				if (c2->type == COLLIDER_PLAYER_SHOT) {
					switch (c1->type)
					{
			
					case COLLIDER_ENEMY_BASIC:
						App->player->player_points += 150;
						break;

					case COLLIDER_ENEMY_BIGTANK:
					case COLLIDER_ENEMY_BIGBOAT:
					case COLLIDER_ENEMY_POWERUPSHIP:
					case COLLIDER_ENEMY_BOSS:

						App->player->player_points += 20;
						break;

					case COLLIDER_ENEMY_GREENSHIP:
					case COLLIDER_ENEMY_TANK:

						App->player->player_points += 40;
						break;

					case COLLIDER_BOX:

						App->player->player_points += 500;
						break;

					default:
						break;
					}
				}

				else if(c2->type == COLLIDER_PLAYER_2_SHOT){
					switch (c1->type)
					{

					case COLLIDER_ENEMY_BASIC:
						App->player2->player_points += 150;
						break;

					case COLLIDER_ENEMY_BIGTANK:
					case COLLIDER_ENEMY_BIGBOAT:
					case COLLIDER_ENEMY_POWERUPSHIP:
					case COLLIDER_ENEMY_BOSS:

						App->player2->player_points += 20;
						break;

					case COLLIDER_ENEMY_GREENSHIP:
					case COLLIDER_ENEMY_TANK:

						App->player2->player_points += 40;
						break;

					case COLLIDER_BOX:

						App->player2->player_points += 500;
						break;

					default:
						break;
					}

				}

				if (c1->life_units <= 0) {
					enemies[i]->OnCollision(c2, c1);
					delete enemies[i];
					enemies[i] = nullptr;
					break;

				}

			}
		}
	}
}
*/