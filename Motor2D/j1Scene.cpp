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
#include "j1Enemies.h"
#include "j1Pathfinding.h"


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

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

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


	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT)
		App->audio->VolumeUp();

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT) 
		App->audio->VolumeDown();

	/*if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 3;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 3;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 3;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 3;*/

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

	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		if (App->framerate_cap != 30)
			App->framerate_cap = 30;
		else
			App->framerate_cap = 0;
	}
	if (App->input->GetKey(SDL_SCANCODE_F12) == KEY_DOWN){}


		

	if (App->player->x - (-App->render->camera.x +(1 *App->render->camera.w / 3)) >= 0) {
		if (App->render->camera.x - App->render->camera.w > -(App->map->data.width*App->map->data.tile_width))
			App->render->camera.x -= 2;
	}

	if (App->player->x - (-App->render->camera.x + (1 * App->render->camera.w / 4)) <= 0) {
		if (App->render->camera.x < 0)
			App->render->camera.x += 2;
	}
	

	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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
	App->player->LoadTexture();
	App->enemies->LoadEnemyText();
	

	if (map == -1) {

		if (CurrentMap->next != nullptr)
			 CurrentMap = CurrentMap->next;
		else
			CurrentMap = MapsList_String.start;		
	}
	else {
		CurrentMap = MapsList_String.start;
		for (int i = 1; i < map; i++) {
			if (CurrentMap->next != nullptr) 
				CurrentMap = CurrentMap->next;
		
			else 
				break;
			}
	
	}
	App->map->Load(CurrentMap->data.GetString());

	App->player->FindSpawn();
	App->player->SpawnPlayer();

	return true;
}


// Load
bool j1Scene::Load(pugi::xml_node&  savegame) {
	currmap = savegame.child("Map").attribute("CurrentMap").as_int();

	App->map->CleanUp();

	switch (currmap)
	{
	case 1:
		App->map->Load("Map1.tmx");
		break;
	case 2:
		App->map->Load("Map2.tmx");
		break;
	default:
		break;
	}
	return true;
}

//Save
bool j1Scene::Save(pugi::xml_node& data) const {

	pugi::xml_node cam = data.append_child("Map");

	cam.append_attribute("CurrentMap") = currmap;
	return true;
}