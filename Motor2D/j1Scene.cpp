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
#include "J1Player.h"
#include "j1Entities.h"
#include "j1Pathfinding.h"



#include <time.h>
#include <chrono>
#include <ctime>
#include <iostream>




j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	for (pugi::xml_node map_node = config.child("maps"); map_node != nullptr; map_node = map_node.next_sibling("maps")) {

		p2SString aux = map_node.attribute("map").as_string();

		MapsList_String.add(aux);
	}

	CurrentMap = MapsList_String.start;

	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(CurrentMap->data);

	LoadWalkabilityMap();

	p2List_item<MapLayer*>* layer = App->map->data.layers.start;
	iPoint size_map;
	size_map = App->map->MapToWorld(layer->data->width, layer->data->height);
	width_map = size_map.x;
	FindEntities();
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT)
		App->audio->VolumeUp();

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT) 
		App->audio->VolumeDown();

	if (Photo_mode) {

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			if (App->render->camera.y < 0)
				App->render->camera.y += 5;

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			if (-(App->render->camera.y/App->win->scale) + (App->render->camera.h /App->win->scale) < App->render->camera.h)
			App->render->camera.y -= 5;

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			if (App->render->camera.x < 0)
			App->render->camera.x += 5;

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			if (-(App->render->camera.x / App->win->scale) + (App->render->camera.w / App->win->scale) < width_map)
			App->render->camera.x -= 5;
	}
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		LoadScene(1);	
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		LoadScene();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		App->SaveGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		App->LoadGame();
	}
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		App->map->debug_draw = !App->map->debug_draw;
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		App->entities->player->god_mode = !App->entities->player->god_mode;
	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		if (App->framerate_cap != 30)
			App->framerate_cap = 30;
		else
			App->framerate_cap = 0;
	}
	if (App->input->GetKey(SDL_SCANCODE_F12) == KEY_DOWN){}


		
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		Photo_mode = !Photo_mode;
		App->render->camera.y = 0;
		App->win->scale = 1;
	}


		


		if ((App->entities->player->x - (-App->render->camera.x + (1 * App->render->camera.w / 3)) >= 0) && !Photo_mode) {
			if (App->render->camera.x - App->render->camera.w > -(App->map->data.width*App->map->data.tile_width))
				App->render->camera.x -= 4;
		}

		if ((App->entities->player->x - (-App->render->camera.x + (1 * App->render->camera.w / 4)) <= 0) && !Photo_mode) {
			if (App->render->camera.x < 0)
				App->render->camera.x += 4;
		}
/*	if ((App->player->y - (-App->render->camera.y + (1 * App->render->camera.h/3)) >= 0) && App->win->scale != 1) {
			App->render->camera.y -= 2;
	}

	if ((App->player->y - (App->render->camera.y + (1 * App->render->camera.h / 2)) <= 0) && App->win->scale != 1) {
			App->render->camera.y += 2;
	}*/
	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && Photo_mode) {

		SDL_RenderReadPixels(App->render->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, App->win->screen_surface->pixels, App->win->screen_surface->pitch);

		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		tm time;
		localtime_s(&time, &now_c);

		sec = time.tm_sec;
		mins = time.tm_min;
		hour = time.tm_hour;
		day = time.tm_mday;
		month = time.tm_mon + 1;
		year = time.tm_year + 1900;

		static char photo_name[60];

		if (sec == last_frame_sec) {
			photos_this_sec++;

			sprintf_s(photo_name, 60, "screenshots/Factory Escape on %i-%02i-%02i at %02i.%02i.%02i(%i).png",
				year, month, day, hour, mins, sec, photos_this_sec);
		}
		else {
			photos_this_sec = 0;

			sprintf_s(photo_name, 60, "screenshots/Factory Escape on %i-%02i-%02i at %02i.%02i.%02i.png",
				year, month, day, hour, mins, sec);
		}

		last_frame_sec = sec;

		LOG("Saving photo %s", photo_name);

		SDL_SaveBMP(App->win->screen_surface, photo_name);
		
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::LoadScene(int map) {

	App->map->CleanUp();
	App->audio->FreeMusic();
	App->tex->FreeTextures();
	//App->entities->player->LoadTexture();
	App->entities->LoadEntityText();
	App->entities->FreeEnemies();

	if (map == -1) {

		if (CurrentMap->next != nullptr) {
			CurrentMap = CurrentMap->next;
			currmap++;
		}
		else {
			CurrentMap = MapsList_String.start;
			currmap = 1;
		}
	}
	else {
		CurrentMap = MapsList_String.start;
		for (int i = 1; i < map; i++) {
			if (CurrentMap->next != nullptr) {
				CurrentMap = CurrentMap->next;
				currmap = map;
			}
			else 
				break;
			}
	
	}

	App->map->Load(CurrentMap->data.GetString());

	LoadWalkabilityMap();
	App->scene->FindEntities();

	App->entities->player->FindSpawn();
	App->entities->player->SpawnPlayer();

	return true;
}


// Load
bool j1Scene::Load(pugi::xml_node&  savegame) {
	currmap = savegame.child("Map").attribute("CurrentMap").as_int();

	LoadScene(currmap);

	return true;
}

//Save
bool j1Scene::Save(pugi::xml_node& data) const {

	pugi::xml_node cam = data.append_child("Map");

	cam.append_attribute("CurrentMap") = currmap;
	return true;
}

void j1Scene::LoadWalkabilityMap() {
	int w, h;
	uchar* data = NULL;

	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);
}


void j1Scene::FindEntities()
{
	p2List_item<MapLayer*>* layer = App->map->data.layers.end;
	if (layer != nullptr) {
		for (int i = 0; i < layer->data->size_data; i++)
		{
			if (layer->data->data[i] == Tile_Type::BAT_SPAWN)
			{
				iPoint spawn = App->map->TileToWorld(i);
				App->entities->AddEntity(ENTITY_TYPES::BAT, spawn.x, spawn.y);
			}
			if (layer->data->data[i] == Tile_Type::BLOP_SPAWN)
			{
				iPoint spawn = App->map->TileToWorld(i);
				App->entities->AddEntity(ENTITY_TYPES::BLOP, spawn.x, spawn.y);
			}
			if (layer->data->data[i] == Tile_Type::PLAYER_SPAWN)
			{
				iPoint spawn = App->map->TileToWorld(i);
				App->entities->AddEntity(ENTITY_TYPES::PLAYER, spawn.x, spawn.y);
			}
		}
	}
}
