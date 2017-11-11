#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Input.h"
#include <math.h>
#include "J1Player.h"
#include "j1Audio.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	music_path = config.child("music").attribute("level").value();

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	SDL_Rect* rect;
	int x, y, h, w;
	int layerss = 0;
	for (p2List_item<TileSet*>* blit_tilesets = data.tilesets.start; blit_tilesets != nullptr; blit_tilesets = blit_tilesets->next) {
		for (p2List_item<MapLayer*>* layer = this->data.layers.start; layer->next != nullptr; layer = layer->next) {
			layerss++;
			x = y = h = w = 0;

			for (int id = 0; id < layer->data->size_data; id++) {
				rect = &blit_tilesets->data->GetTileRect(layer->data->data[id]);

				App->render->Blit(blit_tilesets->data->texture, x, y, rect,layer->data->speed);

				w++;
				if (w == layer->data->width) {
					w = 0;
					h++;
				}

				x = w * blit_tilesets->data->tile_width;
				y = h * blit_tilesets->data->tile_height;
			}
		}
	} 
}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	p2List_item<MapLayer*>* iter;
	iter = data.layers.start;

	while (iter != nullptr)
	{
		RELEASE(iter->data);
		iter = iter->next;
	}

	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();


	return true;
}

// Load new map
bool j1Map::Load(p2SString file_name)
{

//	App->audio->PlayMusic(music_path);

	App->audio->PlayMusic("audio/music/Urban-Jungle-2061_Looping.ogg");

	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name.GetString());

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// TODO 4: Iterate all layers and load each of them
	// Load layer info ----------------------------------------------

	pugi::xml_node layers;
	for (layers = map_file.child("map").child("layer"); layers && ret; layers = layers.next_sibling("layer")) {

		MapLayer* layer = new MapLayer;
		
		if (ret == true) {

			ret = LoadLayer(layers, layer);
	
		}
		data.layers.add(layer);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
		
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name);
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}



	App->render->camera.x = 0;
	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	

	const char* aux = node.child("properties").child("property").attribute("name").as_string();
	
	if (strcmp(aux,"speed") == 0)
		layer->speed = node.child("properties").child("property").attribute("value").as_float(1);
	

	for (pugi::xml_node iterator = node.child("data").child("tile"); iterator != nullptr; iterator = iterator.next_sibling())
	{
		layer->size_data++;
	
	}


	layer->data = new uint[layer->size_data];

	memset(layer->data, 0, layer->size_data);
	int i = 0;
	for (pugi::xml_node iterator = node.child("data").child("tile"); iterator != nullptr; iterator = iterator.next_sibling())
	{
		layer->data[i] = iterator.attribute("gid").as_uint();
			i++;
	}



	return ret;
}

iPoint j1Map::GetPosition(TileSet* tile, int x, int y) {

	iPoint pos;

	pos.x = x / tile->tile_width;
	pos.y = y / tile->tile_width;

	

	return pos;
}

int j1Map::MapPosition(TileSet* tile, int x, int y) {

	p2List_item<MapLayer*>* layer = this->data.layers.start;

	iPoint pos = GetPosition(tile, x, y);
	int ret = (pos.y * layer->data->width) + pos.x;

	return ret;
}


ColisionType j1Map::CheckColision(int gid) {

	ColisionType ret = NONE;
	int colision;

	p2List_item<MapLayer*>* layer = this->data.layers.end;
		
		colision = layer->data->data[gid];
	
			switch (colision) {
			case Tile_Type::OBSTACLES:
				ret = DEATH;
				break;
			case Tile_Type::WALL:
				ret = GROUND;
				break;
			case Tile_Type::LEVEL_END:
				ret = WIN;
			}
			
		return ret;
}

iPoint j1Map::GidToWorld(int Gid)
{
	iPoint pos;
	float x;
	p2List_item<MapLayer*>* layer = this->data.layers.start;

	x = (float)Gid / layer->data->width;
	pos.y = x;
	pos.x = (x - pos.y) * layer->data->width;

	pos.x *= data.tilesets.start->data->tile_width;
	pos.y *= data.tilesets.start->data->tile_height;

	return pos;
}