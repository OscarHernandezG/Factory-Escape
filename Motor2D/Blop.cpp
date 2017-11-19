#include "j1App.h"
#include "Blop.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "J1Player.h"
#include "j1Enemies.h"





Blop::Blop(int x, int y) : Enemy(x, y)
{
	position.x = fpos.x = x;
	position.y = fpos.y = y;
	LoadAnimation();
	CurrentAnim = &Idle;
}

void Blop::Move(float dt)
{
	x = position.x;
	iPoint player_pos;
	iPoint enemy_pos = App->map->GetPosition(App->map->data.tilesets.start->data, position.x, position.y);

	if (enemy_pos == PosToGo || firstpath) {
		iPoint player_pos = App->map->GetPosition(App->map->data.tilesets.start->data, App->player->x, App->player->y + Tile_h);

		App->pathfinding->CreatePath(enemy_pos, player_pos, BLOP);
		path = App->pathfinding->GetLastPath();
		path->Flip();
		path->Pop(PosToGo);
		path->Pop(PosToGo);
		App->pathfinding->ClearLastPath();

		if (PosToGo.x > 0 && PosToGo.y > 0) {
			iPoint next_pos = App->map->MapToWorld(PosToGo.x, PosToGo.y);
			next_pos.x += 30;
			next_pos.y += 30;

			if (position.x + 30 > next_pos.x) {
				if (position.x - next_pos.x <= speed*dt)
					position.x = next_pos.x;
				else
					fpos.x -= speed * dt;
			}
			else if (position.x + 30 < next_pos.x) {
				if (next_pos.x - position.x <= speed*dt)
					position.x = next_pos.x;
				else
					fpos.x += speed * dt;
			}
			if (position.y + 30 < next_pos.y) {
				if (next_pos.y - position.y <= speed*dt)
					position.y = next_pos.y;
				else
					fpos.y += speed * dt;
			}
		}

		position.x = fpos.x;
		position.y = fpos.y;

		flip = SDL_FLIP_NONE;
		if (x == position.x)
			CurrentAnim = &Idle;
		else if (x >= position.x)
			CurrentAnim = &Walk;
		else if (x <= position.x) {
			CurrentAnim = &Walk;
			flip = SDL_FLIP_HORIZONTAL;
		}

	}
}


void Blop::Draw(SDL_Texture* texture) {
	App->render->Blit(texture, position.x, position.y, &CurrentAnim->GetCurrentFrame(), 1, flip);
}


void Blop::LoadAnimation() {

	for (int i = 8; i < 14; i++) {
		Idle.PushBack({ App->enemies->animation_x[i],App->enemies->animation_y[i],App->enemies->animation_w[i],App->enemies->animation_h[i] });
	}
	Idle.loop = true;
	Idle.speed = 1.5f;

	for (int i = 0; i < 8; i++) {
		Walk.PushBack({ App->enemies->animation_x[i],App->enemies->animation_y[i],App->enemies->animation_w[i],App->enemies->animation_h[i] });
	}
	Walk.loop = true;
	Walk.speed = 1.5f;
}

