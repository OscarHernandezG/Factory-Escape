#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"


j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos, ENTITY_TYPES type) const
{
	uchar t = GetTileAt(pos);
	switch (type)
	{
	case BAT:		
		return t != INVALID_WALK_CODE && t > 0;
		break;
	case BLOP:
		return t != INVALID_WALK_CODE && t > 1;
		break;
	default:
		return false;
		break;
	}


}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
p2DynArray<iPoint>* j1PathFinding::GetLastPath() 
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill, ENTITY_TYPES type) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding->IsWalkable(cell, type))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell, type))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell, type))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell, type))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}


// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination, ENTITY_TYPES type)
{
	//if origin or destination are not walkable, return -1
	int ret = -1;
	if (IsWalkable(origin, type) && IsWalkable(destination, type))
	{
		ret = 0;
		// Create two lists: open, close.
		PathList open, close;

		// Add the origin tile to open
		PathNode Origin(1, origin.DistanceTo(destination), origin, nullptr);
		open.list.add(Origin);

		// Iterate while we have tile in the open list
		while (open.list.count() > 0) {

			//  Move the lowest score cell from open list to the closed list

			close.list.add(open.GetNodeLowestScore()->data);
			open.list.del(open.GetNodeLowestScore());

			++ret;

			// If we just added the destination, we are done!
			// Backtrack to create the final path
			// Use the Pathnode::parent and Flip() the path when you are finish

			if (close.list.end->data.pos == destination) {
				last_path.Clear();
				for (p2List_item<PathNode>* iterator = close.list.end; iterator->data.pos != origin; iterator = close.Find(iterator->data.parent->pos)) {

					last_path.PushBack(iterator->data.pos);
				}
				last_path.PushBack(origin);
				last_path .Flip();
				break;
			}

			// Fill a list of all adjancent nodes

			PathList Neighbours;
			close.list.end->data.FindWalkableAdjacents(Neighbours, type);

			// Iterate adjancent nodes:
			for (p2List_item<PathNode>* iterator = Neighbours.list.start; iterator != nullptr; iterator = iterator->next) {
				// ignore nodes in the closed list
				if (close.Find(iterator->data.pos) != NULL)
					continue;

				iterator->data.CalculateF(destination);
				// If it is NOT found, calculate its F and add it to the open list
				p2List_item<PathNode>* old_node = open.Find(iterator->data.pos);
				if (old_node != NULL) {
					// If it is already in the open list, check if it is a better path (compare G)
					if (old_node->data.g > iterator->data.g) {
						// If it is a better path, Update the parent
						old_node->data.g = iterator->data.g;
						old_node->data.parent = iterator->data.parent;
					}
				}
				else {
					open.list.add(iterator->data);
				}
			}
		}
	}
	return ret;
}
