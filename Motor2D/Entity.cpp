#include "j1App.h"
#include "Entity.h"
#include "j1Render.h"
#include "j1Entities.h"

Entity::Entity(int x, int y, ENTITY_TYPES etype) : position(x, y), type(etype)
{
	

	

}

Entity::~Entity()
{

}

void Entity::Draw(SDL_Texture* sprites)
{
	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()),1,SDL_FLIP_NONE,0,255);
}

