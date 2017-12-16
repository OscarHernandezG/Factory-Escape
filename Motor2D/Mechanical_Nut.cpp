#include "j1App.h"
#include "Mechanical_Nut.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1Entities.h"




Mechanical_Nut::Mechanical_Nut(int x, int y, ENTITY_TYPES etype) : Entity(x, y, etype)
{
	position.x = fpos.x = x;
	position.y = fpos.y = y;

	Nut_rect.x = 885;
	Nut_rect.y = 1205;
	Nut_rect.w = Nut_rect.h = 60;
	
}
void Mechanical_Nut::Move(float dt)
{
	angle += dt*100;
}

void Mechanical_Nut::Draw(SDL_Texture* texture) {
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	App->render->Blit(texture, position.x, position.y, &Nut_rect, 1,SDL_FLIP_NONE,angle);

}

