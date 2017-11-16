#include "j1App.h"
#include "Bat.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "J1Player.h"




Bat::Bat(int x, int y) : Enemy(x, y)
{
	position.x = x;
	position.y = y;
	LoadAnimation();
}

void Bat::Move(float dt)
{

	iPoint player_pos =	App->map->GetPosition(App->map->data.tilesets.start->data, App->player->x, App->player->y);
	iPoint enemy_pos = App->map->GetPosition(App->map->data.tilesets.start->data, position.x, position.y);

	App->pathfinding->CreatePath(enemy_pos, player_pos);
	path = App->pathfinding->GetLastPath();

	path->Pop(enemy_pos);

	position = App->map->MapToWorld(enemy_pos.x, enemy_pos.y);
	
	 
	/*position.x--;*/
	CurrentAnim = &Idle;
}

void Bat::Draw(SDL_Texture* texture) {
	App->render->Blit(texture, position.x, position.y, &CurrentAnim->GetCurrentFrame(), 1);
}


void Bat::LoadAnimation() {

	pugi::xml_document	animation_file;
	pugi::xml_parse_result animations = animation_file.load_file("textures/bat_animation.xml");
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

	for (int i = 0; i < 4; i++) {
		Idle.PushBack({ animation_x[i],animation_y[i],animation_w[i],animation_h[i] });
	}
	Idle.loop = true;
	Idle.speed = 1.5f;

	for (int i = 4; i < 8; i++) {
		Left.PushBack({ animation_x[i],animation_y[i],animation_w[i],animation_h[i] });
	}
	Left.loop = true;
	Left.speed = 1.5f;

	for (int i = 8; i < 12; i++) {
		Right.PushBack({ animation_x[i],animation_y[i],animation_w[i],animation_h[i] });
	}
	Right.loop = true;
	Right.speed = 1.5f;
}

