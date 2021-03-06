#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};


struct MapLayer {

	const char* name = nullptr;
	int width, height;
	unsigned int* data;
	uint size_data = 0;
	int property = 0;
	float speed = 1;
	bool Navigation = false;
	Properties	properties;

	~MapLayer() {
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};



// ----------------------------------------------------

inline uint Get(int x, int y) {
	uint res;

	res = (y * 8) + x;

	return res;
}



// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

enum ColisionType
{
	NONE_COL = 0,
	GROUND,
	WIN,

	DEATH
};

enum Tile_Type
{
	OBSTACLES = 29,
	WALL,
	LEVEL_END,
	PLAYER_SPAWN,
	BAT_SPAWN,
	PATHFINDING_BAT,
	BLOP_SPAWN,
	PATHFINDING_BLOP,
	MECHANICAL_NUT_SPAWN
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	Properties			gid_properties;

};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(p2SString path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	iPoint GetPosition(TileSet* tile, int x, int y);

	int MapPosition(TileSet* tile, int x, int y);

	ColisionType CheckColision(int gid);

	iPoint TileToWorld(int Gid);

	
	TileSet* GetTilesetFromTileId(int id) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	bool LoadGidProperties(pugi::xml_node& node);

public:

	MapData data;
	bool col;

	const char* music_path;

	bool debug_draw = false;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__