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

}

