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
#include "Enemy.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");
}

// Destructor
j1Player::~j1Player()
{}

// Called before render is available
bool j1Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	//Load player texture

	LoadTexture();

	LoadAnimations();
	CurrentAnim = &Idle;

	FindSpawn();
	SpawnPlayer();
	
	speed.x = 0;
	speed.y = 0;

	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT) {
		App->win->scale += 0.1f * dt;
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) {
		App->win->scale -= 0.1f * dt;
	}

	if (App->win->scale > 1 || App->input->GetScroll() == 1) 
		App->win->scale += App->input->GetScroll() * dt;		


	if(App->win->scale == 1)
		CheckPlayerState(dt);
	else //Mode Photo
		App->dt = 0;

	switch (PlayerState)
	{
	case IDLE:
		if(CanJump)
		CurrentAnim = &Idle;
		break;
	case RUNNING_RIGHT:
		if (CanJump)
		CurrentAnim = &Run;
		x+= 200 * dt;
		flip = SDL_FLIP_NONE;
		break;
	case RUNNING_LEFT:
		if (CanJump)
		CurrentAnim = &Run;
		x-= 200 * dt;
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

	App->render->Blit(texture, x, y, &CurrentAnim->GetCurrentFrame(),1, flip,0,255);
	PlayerState = IDLE;

	return true;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Player::CleanUp()
{
	LOG("Freeing player");

	delete[] animation_x;
	delete[] animation_y;
	delete[] animation_w;
	delete[] animation_h;

	animations_list.clear();

	return true;
}


// Load
bool j1Player::Load(pugi::xml_node&  data) 
{
	x = data.child("position").attribute("x").as_int();
	y = data.child("position").attribute("y").as_int();

	return true;
}


//Save
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("position");

	cam.append_attribute("x") = x;
	cam.append_attribute("y") = y;

	return true;
}

void j1Player::LoadTexture() 
{
	texture = App->tex->Load("textures/robot_animation.png");
}

void j1Player::CheckPlayerState(float dt) 
{
	
	if (jump) {

		speed.y += down_force * dt;
		CanJump = false;
		pos = App->map->MapPosition(App->map->data.tilesets.start->data, x, y);
		ColisionType colision1 = App->map->CheckColision(pos);
		ColisionType colision2 = App->map->CheckColision(pos + 1);

		if (speed.y == 0 || (colision1 != NONE || colision2 != NONE)) {
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
			y += down_force * (float)dt/9;
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

			colision3 = NONE;

			if (colision1 == NONE && colision2 == NONE && colision3 == NONE) {
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

			colision3 = NONE;

			if (colision1 == NONE && colision2 == NONE && colision3 == NONE) {
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


			if (colision == NONE) {
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

			ColisionType colision = App->map->CheckColision(pos + 3);

			if (colision == NONE) {
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
		if (currentTime > dieTime + 1000) {
			Die.Reset();
			death = false;
			PlayerState = IDLE;

			SpawnPlayer();
		}
	}
	if (!death)
	death = CheckPlayerDeath();

	down_force = 1250;
}

void j1Player::LoadAnimations()
{
	pugi::xml_document	animation_file;
	pugi::xml_parse_result animations = animation_file.load_file("textures/animations.xml");
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

	// Idle Animation

	for (int i = 0; i < 10; i++) {
		Idle.PushBack({ animation_x[i],animation_y[i],animation_w[i],animation_h[i] });
	}
	Idle.loop = true;
	Idle.speed = 1;

	// Jump Animation

	for (int i = 18; i < 28; i++) {
		Jump.PushBack({ animation_x[i],animation_y[i],animation_w[i],animation_h[i] });
	}
	Jump.loop = false;
	Jump.speed = 2;

	// Run Animation

	for (int i = 10; i < 18; i++) {
		Run.PushBack({ animation_x[i],animation_y[i],animation_w[i],animation_h[i] });
	}
	Run.loop = true;
	Run.speed = 2;

	// Slide Animation

	for (int i = 38; i < 48; i++) {
		Slide.PushBack({ animation_x[i],animation_y[i],animation_w[i],animation_h[i] });
	}
	Slide.loop = false;
	Slide.speed = 1;

	//Die Animation

	for (int i = 28; i < 38; i++) {
		Die.PushBack({ animation_x[i],animation_y[i],animation_w[i],animation_h[i] });
	}
	Die.loop = false;
	Die.speed = 2;

	//Melee Animation

	for (int i = 48; i < 57; i++) {
		Melee.PushBack({ animation_x[i],animation_y[i],animation_w[i],animation_h[i] });
	}
	Melee.loop = false;
	Melee.speed = 2;

	//Shoot Animation

	for (int i = 58; i < 68; i++) {
		Shoot.PushBack({ animation_x[i],animation_y[i],animation_w[i],animation_h[i] });
	}
	Shoot.loop = false;
	Shoot.speed = 2;

	MeleeJump = Melee;

}

void j1Player::FindSpawn()
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

void j1Player::SpawnPlayer() {

	x = spawn.x;
	y = spawn.y;
}

void j1Player::KillEnemies() {

	for (uint i = 0; i < MAX_ENEMIES; ++i) {
		if (App->enemies->enemies[i] != nullptr) {

			if (x < App->enemies->enemies[i]->position.x && x + 64 > App->enemies->enemies[i]->position.x) {
				if (y < App->enemies->enemies[i]->position.y && y + 120 > App->enemies->enemies[i]->position.y) {
					delete App->enemies->enemies[i];
					App->enemies->enemies[i] = nullptr;
				}
			}
		}
	}
}

bool j1Player::CheckPlayerDeath() {

	bool ret = false;
	for (uint i = 0; i < MAX_ENEMIES; ++i) {
		if (App->enemies->enemies[i] != nullptr) {
			if (x < App->enemies->enemies[i]->position.x && x + 48 > App->enemies->enemies[i]->position.x) {
				if (y < App->enemies->enemies[i]->position.y && y + 120 > App->enemies->enemies[i]->position.y) {
					ret = true;
					break;

				}
			}
		}
	}
	return ret;
}
