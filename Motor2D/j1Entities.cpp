#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Entities.h"
#include "Bat.h"
#include "Blop.h"
#include "Player.h"
#include "j1Menu.h"



#define SPAWN_MARGIN 50

j1Entities::j1Entities()
{

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
	
	if (!App->scene->Photo_mode)
		for (p2List_item<Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
			iterator->data->Move(dt);

	for (p2List_item<Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
		iterator->data->Draw(sprites);
	
	return true;
}

bool j1Entities::PostUpdate()
{
	// check camera position to decide what to spawn
	for (p2List_item<Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{

		if (-iterator->data->position.y/*screen_Size*/ < (App->render->camera.y) - SPAWN_MARGIN * 16)
		{
			LOG("DeSpawning enemy at %d", iterator->data->position.x /* * screen_Size*/);
			delete iterator->data;
			entities.del(iterator);
		}

	}

	return true;
}

// Called before quitting
bool j1Entities::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(sprites);
	int i = 0;
	for (p2List_item<Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		delete iterator->data;
		entities.del(iterator);

		if (i < MAX_ENEMIES)
			queue[i].type = NO_TYPE;
		i++;
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

	int i = 0;
	for (p2List_item<Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		delete iterator->data;
		entities.del(iterator);

		if (i < MAX_ENEMIES)
			queue[i].type = NO_TYPE;
		i++;
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

	Entity* new_entity = nullptr;

	switch (info.type)
	{
	case ENTITY_TYPES::BAT:
		new_entity = new Bat(info.x, info.y, info.type);
		break;
	case ENTITY_TYPES::BLOP:
		new_entity = new Blop(info.x, info.y, info.type);
		break;
	case ENTITY_TYPES::PLAYER:
		new_entity = new Player(info.x, info.y, info.type);
		player = (Player*)new_entity;
		break;
	}

	if (new_entity != nullptr) {
		entities.add(new_entity);
	}

}

bool j1Entities::Load(pugi::xml_node&  data)
{
	for (p2List_item<Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next) 
		delete iterator->data;

		entities.clear();
		pugi::xml_node entity;
		for (entity = data.child("position"); entity; entity = entity.next_sibling("position")) {

			EntityInfo info;
			info.x = entity.attribute("x").as_int();
			info.y = entity.attribute("y").as_int();
			info.type = (ENTITY_TYPES)entity.attribute("type").as_int();

			SpawnEnemy(info);
		}
	
	return true;
}


//Save
bool j1Entities::Save(pugi::xml_node& data) const
{
	for (p2List_item<Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next) {

		pugi::xml_node entity = data.append_child("position");

		entity.append_attribute("x") = iterator->data->position.x;
		entity.append_attribute("y") = iterator->data->position.y;
		entity.append_attribute("type") = iterator->data->type;
	}
	return true;
}
