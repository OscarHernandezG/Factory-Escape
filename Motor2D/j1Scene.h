#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2List.h"

struct SDL_Texture;


class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool LoadScene(int map = -1);

	// Load
	bool Load(pugi::xml_node&  savegame);

	//Save
	bool Save(pugi::xml_node& data) const;
	
	int currmap = 1;
	//char* Screenshoot_name = ("Screen %i.png");
private:

	uint width_map;

	int  sec, mins, hour, day, month, year;
	int last_frame_sec = -1;
	int photos_this_sec = 0;

	/*p2List<char>* Screenshoot_name;
	uint cont_screenshots = 1;*/
	p2List<p2SString>  MapsList_String;
	p2List_item<p2SString>* CurrentMap = nullptr;

};

#endif // __j1SCENE_H__