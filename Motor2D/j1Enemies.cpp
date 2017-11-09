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

	return true;
}

bool j1Enemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			/*if (-queue[i].y < (App->render->camera.y/SCREEN_SIZE) + SPAWN_MARGIN / 2)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].x * SCREEN_SIZE);

			}*/

		}
	}



	return true;
}

// Called before render is available
bool j1Enemies::Update()
{

	/*for (uint i = 0; i < 10; ++i)
		if (enemies[i] != nullptr) { enemies[i]->Draw(sprites); }

	for (uint i = 0; i < 10; ++i)
		if (enemies[i] != nullptr) { enemies[i]->Move(); }

	


	if (App->player->homing_shot) {

		int target = 0;
		if (need_target) {
			for (uint i = 0; i < MAX_ENEMIES - 1; ++i) {

				if (enemies[target] != nullptr && enemies[i + 1] != nullptr) {


					int enemy1_x = (App->particles->homing_missile.position.x - enemies[target]->position.x);
					int enemy1_y = (App->particles->homing_missile.position.y - enemies[target]->position.y);

					int enemy2_x = (App->particles->homing_missile.position.x - enemies[i]->position.x);
					int enemy2_y = (App->particles->homing_missile.position.y - enemies[i]->position.y);

					int enemy_1 = pow(enemy1_x, 2) + pow(enemy1_y, 2);
					int enemy_2 = pow(enemy2_x, 2) + pow(enemy2_y, 2);


					if (enemy_1 <= enemy_2) {
						target = i;
					}

				}
			}
		}

		if (!need_target && enemies[target] != nullptr) {
			speed_x_mult = (enemies[target]->position.x - App->particles->homing_missile.position.x);
			speed_y_mult = (enemies[target]->position.y - App->particles->homing_missile.position.y);

			a_mult = sqrt((pow(speed_x_mult, 2) + pow(speed_y_mult, 2)));

			common_mult = (proj_speed / a_mult);

			App->particles->movement_x = common_mult * speed_x_mult;
			App->particles->movement_y = common_mult * speed_y_mult;

			if (enemies[target] == nullptr) {
				need_target = true;
			}

			//		App->player->homing_shot = false;
		}
	}

	*/
	return true;
}

bool j1Enemies::PostUpdate()
{
	// check camera position to decide what to spawn
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			/*if (-enemies[i]->position.y *SCREEN_SIZE < (App->render->camera.y) - SPAWN_MARGIN * 16)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);
				delete enemies[i];
				enemies[i] = nullptr;
			}*/
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

bool j1Enemies::AddEnemy(ENEMY_TYPES type, int x, int y, int path)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			queue[i].path = path;
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
		/*switch (info.type)
		{
		case ENEMY_TYPES::BASICENEMY:
			enemies[i] = new BasicEnemy(info.x, info.y);
			break;
		case ENEMY_TYPES::GREENSHIP:
			enemies[i] = new GreenShip(info.x, info.y);
			break;
		}	*/		
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