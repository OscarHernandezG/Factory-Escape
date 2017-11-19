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
#include "Blop.h"



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
	LoadEnemyText();
	LoadEnemyAnim();
	if (sprites != nullptr) {
		LOG("Cargado");
	}
	FindEnemies();
	return true;
}

void j1Enemies::LoadEnemyText() {
	sprites = App->tex->Load("textures/enemies.png");
}

void j1Enemies::LoadEnemyAnim() {
	pugi::xml_document	animation_file;
	pugi::xml_parse_result animations = animation_file.load_file("textures/enemies_animation.xml");
	pugi::xml_node SpriteMapping = animation_file.child("SpriteMapping");

	for (pugi::xml_node iterator = SpriteMapping.child("Sprites").child("Sprite"); iterator != nullptr; iterator = iterator.next_sibling("Sprite"))
	{
		int temp_animation_x = iterator.child("Coordinates").child("X").text().as_int();
		int temp_animation_y = iterator.child("Coordinates").child("Y").text().as_int();
		int temp_animation_w = iterator.child("Coordinates").child("Width").text().as_int();
		int temp_animation_h = iterator.child("Coordinates").child("Height").text().as_int();

		animations_list.add({ temp_animation_x,temp_animation_y,temp_animation_w,temp_animation_h });

		size++;
	}


	animation_x = new int[size];
	animation_y = new int[size];
	animation_w = new int[size];
	animation_h = new int[size];

	int i = 0;
	p2List_item<SDL_Rect>* aux = animations_list.start;

	while (aux != nullptr) {

		animation_x[i] = aux->data.x;
		animation_y[i] = aux->data.y;
		animation_w[i] = aux->data.w;
		animation_h[i] = aux->data.h;

		aux = aux->next;
		i++;

	}
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
bool j1Enemies::Update(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);
	if (!App->scene->Photo_mode)
		for (uint i = 0; i < MAX_ENEMIES; ++i)
			if (enemies[i] != nullptr) enemies[i]->Move(dt);

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
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENEMY_TYPES::BAT:
			enemies[i] = new Bat(info.x, info.y);
			break;
		case ENEMY_TYPES::BLOP:
			enemies[i] = new Blop(info.x, info.y);
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
		if (layer->data->data[i] == Tile_Type::BLOP_SPAWN)
		{
			iPoint spawn = App->map->TileToWorld(i);
			AddEnemy(ENEMY_TYPES::BLOP, spawn.x, spawn.y);
		}
	}

}
