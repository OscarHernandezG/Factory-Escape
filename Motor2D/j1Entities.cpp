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
#include "j1Entities.h"
#include "Bat.h"
#include "Blop.h"
#include "Player.h"
#include "j1Menu.h"



#define SPAWN_MARGIN 50

j1Entities::j1Entities()
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		entities[i] = nullptr;
}

// Destructor
j1Entities::~j1Entities()
{
}

bool j1Entities::Start()
{
	name.create("entities");
	LoadEntityText();
	LoadEntityAnim();

	return true;
}

void j1Entities::LoadEntityText() {
	sprites = App->tex->Load("textures/Entities.png");
}

void j1Entities::LoadEntityAnim() {
	pugi::xml_document	animation_file;
	pugi::xml_parse_result animations = animation_file.load_file("textures/entities_animation.xml");
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

bool j1Entities::PreUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPES::NO_TYPE)
		{
			if (-queue[i].y < (App->render->camera.y) + SPAWN_MARGIN / 2)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENTITY_TYPES::NO_TYPE;
				LOG("Spawning entity at %d", queue[i].x);

			}
		}
	}
	return true;
}

// Called before render is available
bool j1Entities::Update(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);
	if (App->menu->Started) {
		if (!App->scene->Photo_mode)
			for (uint i = 0; i < MAX_ENEMIES; ++i)
				if (entities[i] != nullptr) entities[i]->Move(dt);

		for (uint i = 0; i < MAX_ENEMIES; ++i)
			if (entities[i] != nullptr) entities[i]->Draw(sprites);
	}
	return true;
}

bool j1Entities::PostUpdate()
{
	// check camera position to decide what to spawn
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(entities[i] != nullptr)
		{
			if (-entities[i]->position.y/*screen_Size*/ < (App->render->camera.y) - SPAWN_MARGIN * 16)
			{
				LOG("DeSpawning enemy at %d", entities[i]->position.x /* * screen_Size*/);
				delete entities[i];
				entities[i] = nullptr;
			}
		}
	}

	return true;
}

// Called before quitting
bool j1Entities::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(sprites);

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
		queue[i].type = NO_TYPE;
	}
	delete[] animation_x;
	delete[] animation_y;
	delete[] animation_w;
	delete[] animation_h;

	animations_list.clear();
	return true;
}

bool j1Entities::FreeEnemies()
{

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
		queue[i].type = NO_TYPE;
	}

	return true;
}

bool j1Entities::AddEntity(ENTITY_TYPES type, int x, int y)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(queue[i].type == ENTITY_TYPES::NO_TYPE)
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

void j1Entities::SpawnEnemy(const EntityInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for (; entities[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENTITY_TYPES::BAT:
			entities[i] = new Bat(info.x, info.y);
			break;
		case ENTITY_TYPES::BLOP:
			entities[i] = new Blop(info.x, info.y);
			break;
		case ENTITY_TYPES::PLAYER:
			entities[i] = new Player(info.x, info.y);
			player = (Player*)entities[i];
			break;
		}
	}

}

bool j1Entities::Load(pugi::xml_node&  data)
{
	player->x = data.child("position").attribute("x").as_int();
	player->y = data.child("position").attribute("y").as_int();

	return true;
}


//Save
bool j1Entities::Save(pugi::xml_node& data) const
{
	pugi::xml_node Player = data.append_child("position");

	Player.append_attribute("x") = player->x;
	Player.append_attribute("y") = player->y;

	return true;
}
