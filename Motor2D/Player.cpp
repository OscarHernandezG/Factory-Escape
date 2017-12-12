#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "Player.h"
#include "j1Scene.h"
#include "j1Entities.h"
#include "Animation.h"
#include "Entity.h"
#include "j1Timer.h"



Player::Player(int x, int y) : Entity(x, y)
{
	//name.create("player");
	LoadAnimations();
	CurrentAnim = &Idle;


	FindSpawn();
	SpawnPlayer();

	speed.x = 0;
	speed.y = 0;
}

void Player::Move(float dt) {

	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	if (App->scene->Photo_mode) {
		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
			App->win->scale += 0.1f * App->zoom_dt;

		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
			App->win->scale -= 0.1f * App->zoom_dt;

		App->win->scale += App->input->GetScroll() * App->zoom_dt;

		if (App->win->scale < 1)
			App->win->scale = 1;
	}

	else
		CheckPlayerState(dt);

	switch (PlayerState)
	{
	case IDLE:
		if (CanJump)
			CurrentAnim = &Idle;
		break;
	case RUNNING_RIGHT:
		if (CanJump)
			CurrentAnim = &Run;
		x += 200 * dt;
		flip = SDL_FLIP_NONE;
		break;
	case RUNNING_LEFT:
		if (CanJump)
			CurrentAnim = &Run;
		x -= 200 * dt;
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case JUMPING_RIGHT:
		CurrentAnim = &Jump;
		flip = SDL_FLIP_NONE;
		speed.y = -jump_force;
		jump = true;
		break;
	case JUMPING_LEFT:
		CurrentAnim = &Jump;
		flip = SDL_FLIP_HORIZONTAL;
		speed.y = -jump_force;
		jump = true;
		break;
	case SHIFT_RIGHT:
		CurrentAnim = &Slide;
		flip = SDL_FLIP_NONE;
		x += 600 * dt;
		break;
	case SHIFT_LEFT:
		CurrentAnim = &Slide;
		flip = SDL_FLIP_HORIZONTAL;
		x -= 600 * dt;
		break;
	case MELEE:
		CurrentAnim = &Melee;
		KillEnemies();
		break;
	case MELEE_RIGHT:
		CurrentAnim = &Melee;

		x += 200 * dt;
		flip = SDL_FLIP_NONE;
		break;
	case MELEE_LEFT:
		CurrentAnim = &Melee;

		x -= 200 * dt;
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case MELEE_JUMP_RIGHT:
		//	CurrentAnim = &MeleeJump;

		break;
	case MELEE_JUMP_LEFT:
		//	CurrentAnim = &MeleeJump;

		break;
	case SHOOT_RIGHT:
		CurrentAnim = &Shoot;

		break;
	case SHOOT_LEFT:
		CurrentAnim = &Shoot;

		break;
	case PLAYER_WIN:
		App->scene->LoadScene();
		SpawnPlayer();
		break;
	case DEAD:
		CurrentAnim = &Die;
		death = true;
		break;
	default:
		break;
	}

	//App->render->Blit(App->entities->sprites, x, y, &CurrentAnim->GetCurrentFrame(), 1, flip, 0, 255);
	PlayerState = IDLE;

}


void Player::Draw(SDL_Texture* texture) {
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	App->render->Blit(texture, x, y, &CurrentAnim->GetCurrentFrame(), 1, flip);
}
// Destructor
Player::~Player()
{}




// Load
/*bool Player::Load(pugi::xml_node&  data)
{
	x = data.child("position").attribute("x").as_int();
	y = data.child("position").attribute("y").as_int();

	return true;
}


//Save
bool Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node player = data.append_child("position");

	player.append_attribute("x") = x;
	player.append_attribute("y") = y;

	return true;
}
*/

void Player::CheckPlayerState(float dt)
{

	if (jump) {

		speed.y += down_force * dt;
		CanJump = false;
		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y);
		ColisionType colision1 = App->map->CheckColision(pos);
		ColisionType colision2 = App->map->CheckColision(pos + 1);

		if (speed.y == 0 || (colision1 != NONE_COL || colision2 != NONE_COL)) {
			jump = false;
		}

		if ((colision1 == DEATH || colision2 == DEATH))
			PlayerState = DEAD;

		y += speed.y  * dt;
		y += down_force * dt;

		if (PlayerState == RUNNING_LEFT) {
			PlayerState == JUMPING_LEFT;
		}
		else if (PlayerState == RUNNING_RIGHT) {
			PlayerState == JUMPING_RIGHT;
		}
	}



	// "Gravity"

	pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y + 118);

	ColisionType colision1 = App->map->CheckColision(pos);
	ColisionType colision2 = App->map->CheckColision(pos + 1);


	if (colision1 != GROUND && colision2 != GROUND) {
		y += down_force * (float)dt / 9;
	}
	if (colision1 == DEATH && colision2 == DEATH) {
		PlayerState = DEAD;
	}
	else if (colision1 == GROUND || colision2 == GROUND) {
		CanJump = true;
		jump = false;
		Jump.Reset();
	}
	//

	if (death == false) {
		dieTime = SDL_GetTicks();
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

			pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y);

			ColisionType colision1 = App->map->CheckColision(pos);
			ColisionType colision2 = App->map->CheckColision(pos + App->map->data.layers.start->data->width);
			ColisionType colision3 = App->map->CheckColision(pos + 2 * (App->map->data.layers.start->data->width));

			colision3 = NONE_COL;

			if (colision1 == NONE_COL && colision2 == NONE_COL && colision3 == NONE_COL) {
				PlayerState = RUNNING_LEFT;
			}
			else if (colision1 == DEATH && colision2 == DEATH) {
				PlayerState = DEAD;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			pos = App->map->MapPosition(App->map->data.tilesets.start->data, x + 64, y);

			ColisionType colision1 = App->map->CheckColision(pos);
			ColisionType colision2 = App->map->CheckColision(pos + App->map->data.layers.start->data->width);
			ColisionType colision3 = App->map->CheckColision(pos + 2 * (App->map->data.layers.start->data->width));

			colision3 = NONE_COL;

			if (colision1 == NONE_COL && colision2 == NONE_COL && colision3 == NONE_COL) {
				PlayerState = RUNNING_RIGHT;
			}
			else if (colision1 == GROUND || colision2 == GROUND) {
				if (colision1 != DEATH && colision2 != DEATH) {
					PlayerState = IDLE;
				}
				else
					PlayerState = DEAD;
			}
			if (colision1 == WIN) {
				PlayerState = PLAYER_WIN;
			}

		}

		if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && CanJump) {

			pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y);

			ColisionType colision = App->map->CheckColision(pos);


			if (colision == NONE_COL) {
				if (PlayerState == RUNNING_LEFT) {
					PlayerState = JUMPING_LEFT;
				}
				else {
					PlayerState = JUMPING_RIGHT;
				}
			}
			else if (colision == GROUND) {
				PlayerState = IDLE;
			}
			else
				PlayerState = DEAD;

		}

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && !melee) {

			melee = true;
		}

		if ((App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) && slide == false && CanJump == true) {


			pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y + 118 / 2);

			ColisionType colision = App->map->CheckColision(pos + 5/*5 Tileds*/);

			if (colision == NONE_COL) {
				slide = true;
				lastTime = SDL_GetTicks();
				y = y + App->map->data.tile_width / 2;
				if (PlayerState == RUNNING_LEFT) {
					PlayerState = SHIFT_LEFT;
				}
				else {
					PlayerState = SHIFT_RIGHT;
				}
			}
			else if (colision == GROUND) {
				PlayerState = IDLE;
			}
			else
				PlayerState = DEAD;
		}


		if (slide) {
			currentTime = SDL_GetTicks();
			if (currentTime > lastTime + 500) {
				slide = false;
				y = y - App->map->data.tile_width / 2;
			}
			else {
				if (PlayerState == RUNNING_LEFT) {
					PlayerState = SHIFT_LEFT;
				}
				else {
					PlayerState = SHIFT_RIGHT;
				}
			}
		}

		else if (melee) {

			if (PlayerState == RUNNING_LEFT)
				PlayerState = MELEE_LEFT;

			else if (PlayerState == RUNNING_RIGHT)
				PlayerState = MELEE_RIGHT;

			else if (PlayerState == JUMPING_LEFT)
				PlayerState = MELEE_JUMP_LEFT;

			else if (PlayerState == JUMPING_RIGHT)
				PlayerState = MELEE_JUMP_RIGHT;

			else
				PlayerState = MELEE;

			if (Melee.Finished()) {
				melee = false;
				Melee.Reset();
				Melee.ResetLoops();
				MeleeJump.Reset();
				MeleeJump.ResetLoops();
			}
		}
	}


	else if (death) {
		currentTime = SDL_GetTicks();
		PlayerState = DEAD;
		y += (down_force * dt) / 10;
		if (currentTime > dieTime + 1000) {
			CurrentAnim = &Idle;
			melee = false;
			App->entities->FreeEnemies();
			App->scene->FindEntities();
			Die.Reset();
			death = false;
			App->render->camera.x = 0;
			PlayerState = IDLE;

			SpawnPlayer();
		}
	}
	if (!death && !god_mode)
		death = CheckPlayerDeath();

	down_force = 1250;
}

void Player::LoadAnimations()
{
// Idle Animation

	for (int i = 0; i < 10; i++) {
		Idle.PushBack({ App->entities->animation_x[i],App->entities->animation_y[i],App->entities->animation_w[i],App->entities->animation_h[i] });
	}
	Idle.loop = true;
	Idle.speed = 1;

	// Jump Animation

	for (int i = 18; i < 28; i++) {
		Jump.PushBack({ App->entities->animation_x[i],App->entities->animation_y[i],App->entities->animation_w[i],App->entities->animation_h[i] });
	}
	Jump.loop = false;
	Jump.speed = 2;

	// Run Animation

	for (int i = 10; i < 18; i++) {
		Run.PushBack({ App->entities->animation_x[i],App->entities->animation_y[i],App->entities->animation_w[i],App->entities->animation_h[i] });
	}
	Run.loop = true;
	Run.speed = 2;

	// Slide Animation

	for (int i = 38; i < 48; i++) {
		Slide.PushBack({ App->entities->animation_x[i],App->entities->animation_y[i],App->entities->animation_w[i],App->entities->animation_h[i] });
	}
	Slide.loop = false;
	Slide.speed = 1;

	//Die Animation

	for (int i = 28; i < 38; i++) {
		Die.PushBack({ App->entities->animation_x[i],App->entities->animation_y[i],App->entities->animation_w[i],App->entities->animation_h[i] });
	}
	Die.loop = false;
	Die.speed = 2;

	//Melee Animation

	for (int i = 48; i < 57; i++) {
		Melee.PushBack({ App->entities->animation_x[i],App->entities->animation_y[i],App->entities->animation_w[i],App->entities->animation_h[i] });
	}
	Melee.loop = false;
	Melee.speed = 2;

	//Shoot Animation

	for (int i = 58; i < 68; i++) {
		Shoot.PushBack({ App->entities->animation_x[i],App->entities->animation_y[i],App->entities->animation_w[i],App->entities->animation_h[i] });
	}
	Shoot.loop = false;
	Shoot.speed = 2;

	MeleeJump = Melee;

}

void Player::FindSpawn()
{
	p2List_item<MapLayer*>* layer = App->map->data.layers.end;
	for (int i = 0; i < layer->data->size_data; i++)
	{
		if (layer->data->data[i] == Tile_Type::PLAYER_SPAWN)
		{
			spawn = App->map->TileToWorld(i);
		}
	}

}

void Player::SpawnPlayer() {

	x = spawn.x;
	y = spawn.y;
}

void Player::KillEnemies() {

	for (uint i = 0; i < MAX_ENEMIES; ++i) {

		if (App->entities->entities[i] != nullptr) {
			if (flip == SDL_FLIP_NONE) {
				if (x < App->entities->entities[i]->position.x && x + 128 > App->entities->entities[i]->position.x) {
					if (y < App->entities->entities[i]->position.y && y + 120 > App->entities->entities[i]->position.y) {
						delete App->entities->entities[i];
						App->entities->entities[i] = nullptr;
					}
				}
			}

			else {
				if (x - 128 < App->entities->entities[i]->position.x && x > App->entities->entities[i]->position.x)
					if (y < App->entities->entities[i]->position.y && y + 120 > App->entities->entities[i]->position.y) {
						delete App->entities->entities[i];
						App->entities->entities[i] = nullptr;
					}
			}
		}
	}
}

bool Player::CheckPlayerDeath() {

	bool ret = false;
	for (uint i = 0; i < MAX_ENEMIES; ++i) {
		if (App->entities->entities[i] != nullptr) {
			if (x < App->entities->entities[i]->position.x && x + 48 > App->entities->entities[i]->position.x) {
				if (y < App->entities->entities[i]->position.y && y + 120 > App->entities->entities[i]->position.y) {
					ret = true;
					break;

				}
			}
		}
	}
	return ret;
}
