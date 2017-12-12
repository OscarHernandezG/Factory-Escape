#include "j1App.h"
#include "Bat.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1Entities.h"




Bat::Bat(int x, int y) : Entity(x, y)
{
	position.x = fpos.x = x;
	position.y = fpos.y = y;
	LoadAnimation();
	CurrentAnim = &Idle;
}

void Bat::Move(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	x = position.x;
	iPoint player_pos = App->map->GetPosition(App->map->data.tilesets.start->data, App->entities->player->x, App->entities->player->y + 60);
	iPoint enemy_pos = App->map->GetPosition(App->map->data.tilesets.start->data, position.x, position.y);
	if (enemy_pos == PosToGo || firstpath) {


		if (App->pathfinding->CreatePath(enemy_pos, player_pos, BAT) > 0) {
			firstpath = false;
		}
		path = App->pathfinding->GetLastPath();
		path->Flip();
		path->Pop(PosToGo);
		path->Pop(PosToGo);

		App->pathfinding->ClearLastPath();


		pf.Start();
	}
	if (PosToGo.x > 0 && PosToGo.y > 0) {
		iPoint next_pos = App->map->MapToWorld(PosToGo.x, PosToGo.y);
		next_pos.x += 30;
		next_pos.y += 30;



		if (abs(player_pos.x - enemy_pos.x) < 4) {

			if (position.x > next_pos.x) {
				if (position.x - next_pos.x <= speed*dt)
					position.x = next_pos.x;
				else
					fpos.x -= speed * dt;
			}
			else if (position.x < next_pos.x) {
				if (next_pos.x - position.x <= speed*dt)
					position.x = next_pos.x;
				else
					fpos.x += speed * dt;
			}
			if (position.y < next_pos.y) {
				if (next_pos.y - position.y <= speed*dt)
					position.y = next_pos.y;
				else
					fpos.y += speed * dt;
			}
			else if (position.y > next_pos.y) {
				if (position.y - next_pos.y <= speed*dt)
					position.y = next_pos.y;
				else
					fpos.y -= speed * dt;
			}
		}
	}
	position.x = fpos.x;
	position.y = fpos.y;

	if (x == position.x)
		CurrentAnim = &Idle;
	else if (x >= position.x)
		CurrentAnim = &Left;
	else if (x <= position.x)
		CurrentAnim = &Right;



}

void Bat::Draw(SDL_Texture* texture) {
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);
	
	App->render->Blit(texture, position.x, position.y, &CurrentAnim->GetCurrentFrame(), 1);
}


void Bat::LoadAnimation() {

	for (int i = 96; i < 100; i++) {
		Idle.PushBack({ App->entities->animation_x[i],App->entities->animation_y[i],App->entities->animation_w[i],App->entities->animation_h[i] });
	}
	Idle.loop = true;
	Idle.speed = 1.5f;

	for (int i = 100; i < 104; i++) {
		Left.PushBack({ App->entities->animation_x[i],App->entities->animation_y[i],App->entities->animation_w[i],App->entities->animation_h[i] });
	}
	Left.loop = true;
	Left.speed = 1.5f;

	for (int i = 104; i < 108; i++) {
		Right.PushBack({ App->entities->animation_x[i],App->entities->animation_y[i],App->entities->animation_w[i],App->entities->animation_h[i] });
	}
	Right.loop = true;
	Right.speed = 1.5f;
}

