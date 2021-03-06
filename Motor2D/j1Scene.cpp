#include "p2Defs.h"
#include "p2Log.h"


#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Entities.h"
#include "j1Pathfinding.h"
#include "j1FadeToBlack.h"
#include "j1Menu.h"
#include "j1Gui.h"
#include "j1Fonts.h"


#include "UI.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_Label.h"
#include "UI_Window.h"
#include "UI_Score.h"
#include "UI_Slider.h"


#include <time.h>
#include <chrono>
#include <ctime>
#include <iostream>




j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	for (pugi::xml_node map_node = config.child("maps"); map_node != nullptr; map_node = map_node.next_sibling("maps")) {

		p2SString aux = map_node.attribute("map").as_string();

		MapsList_String.add(aux);
	}

	camera_sound_fx = App->audio->LoadFx("audio/fx/photo_camera_sound.wav");

	CurrentMap = MapsList_String.start;

//	time_of_start = 0;
	
	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if (App->menu->Started) {
		App->map->Load(CurrentMap->data);

		LoadWalkabilityMap();

		p2List_item<MapLayer*>* layer = App->map->data.layers.start;
		iPoint size_map;
		size_map = App->map->MapToWorld(layer->data->width, layer->data->height);
		width_map = size_map.x;

		FindEntities();

		score = (Score*)App->gui->AdHUDElement(1100, 650, SCORE);
		static char score_text[3];
		sprintf_s(score_text, 3, "%02i", score_nums);
		score->Define({ 156,178,43,44 }, score_text,{61,178,44,44});

		pause_butt = (Button*)App->gui->AdHUDElement(1200, 650, BUTTON);
		pause_butt->Define({ 48,318,49,49 }, { 147,318,49,49 }, { 256,318,49,49 }, "");
		pause_butt->AddListener(this);
		pause_butt->TAB = -1;

		App->font->Load(DEFAULT_FONT,22);

		Timer = (Label*)App->gui->AdHUDElement(20, 20, LABEL);
		static char score_timer[6];
		sprintf_s(score_timer, 6, "%02i:%02i",0,0);
		Timer->SetText(score_timer,1);

		Timer_play.Start();
		cont_timer_pause = 0u;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	if (need_load_scene) {

		LoadScene(currmap, true);
		need_load_scene = false;
	}

	if (App->menu->need_load) {
		App->menu->need_load = false;
		App->LoadGame(App->menu->load_map);
	}
	if (saved && need_save) {
		saved = need_save = false;
		App->gui->active = true;
	}
	if (need_save) {
		App->SaveGame(save_game);
		saved = true;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);


	if (!Pause) {

			static char score_timer[6];
			in_game_time = time_of_start + ((uint)Timer_play.ReadSec() - cont_timer_pause);
			sprintf_s(score_timer, 6, "%02i:%02i", in_game_time / 60, in_game_time % 60);
			Timer->SetText(score_timer, 1);

	}
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		score->ChangeImage();

	if (change_score) {
		score_anim = true;
		change_score = false;
		static char score_text[3];
		sprintf_s(score_text, 3, "%02i", score_nums);
		score->ChangeLabel(score_text);
	}

	if (App->menu->Started && !App->fade->IsFading()) {
		if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT)
			App->audio->VolumeUp();

		if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT)
			App->audio->VolumeDown();

		if (Photo_mode) {

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
				if (App->render->camera.y < 0)
					App->render->camera.y += 5;

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
				if (-(App->render->camera.y / App->win->scale) + (App->render->camera.h / App->win->scale) < App->render->camera.h)
					App->render->camera.y -= 5;

			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
				if (App->render->camera.x < 0)
					App->render->camera.x += 5;

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
				if (-(App->render->camera.x / App->win->scale) + (App->render->camera.w / App->win->scale) < width_map)
					App->render->camera.x -= 5;


			if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
				App->win->scale += 0.1f * App->zoom_dt;

			if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
				App->win->scale -= 0.1f * App->zoom_dt;

			App->win->scale += App->input->GetScroll() * App->zoom_dt;

			if (App->win->scale < 1)
				App->win->scale = 1;

		}

		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
			App->fade->FadeToBlack(this, this, false, 1.0f, 1);
		}

		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
			App->fade->FadeToBlack(this, this, false, 1.0f);
		}

		if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
			App->SaveGame();
		}

		if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
			App->LoadGame();
		}
		if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
			App->menu->debug = !App->menu->debug;
		}
		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
			App->map->debug_draw = !App->map->debug_draw;
		}

		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
			App->entities->player->god_mode = !App->entities->player->god_mode;
		}

		if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
			if (App->current_framerate_cap != App->framerate_cap)
				App->current_framerate_cap = App->framerate_cap;
			else
				App->current_framerate_cap = 0;
		}
		if (App->input->GetKey(SDL_SCANCODE_F12) == KEY_DOWN) {}



		if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
			Photo_mode = !Photo_mode;
			draw_ui = !draw_ui;

			Pause = Photo_mode;
			App->gui->active = !Pause;
			if (in_game_menu && !Pause) 
				Pause = true;

			App->render->camera.y = 0;
			App->win->scale = 1;
		}



		if ((App->entities->player->position.x - (-App->render->camera.x + (1 * App->render->camera.w / 3)) >= 0) && !Photo_mode) {
			if (App->render->camera.x - App->render->camera.w > -(App->map->data.width*App->map->data.tile_width))
				App->render->camera.x -= 4;
		}

		if ((App->entities->player->position.x - (-App->render->camera.x + (1 * App->render->camera.w / 4)) <= 0) && !Photo_mode) {
			if (App->render->camera.x < 0)
				App->render->camera.x += 4;
		}

	}
	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	//App->gui->active = true;
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);
	bool ret = true;

	if (reload_map) {
		App->fade->FadeToBlack(this, this, false, 1.0f, currmap);
		reload_map = false;
	}

	if (next_map) {
		App->fade->FadeToBlack(this, this, false, 1.0f);
		next_map = false;
	}

	if((App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || Quit) && can_quit)
		ret = false;

	if ((App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN || click_PauseButt) && !StartCont_Menu) {
		click_PauseButt = false;
		StartCont_Menu = true;
		Timer_Pause.Start();
		OpenInGameMenu();
		can_quit = !can_quit;
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && Photo_mode) {

		SDL_RenderReadPixels(App->render->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, App->win->screen_surface->pixels, App->win->screen_surface->pitch);

		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		tm time;
		localtime_s(&time, &now_c);

		sec = time.tm_sec;
		mins = time.tm_min;
		hour = time.tm_hour;
		day = time.tm_mday;
		month = time.tm_mon + 1;
		year = time.tm_year + 1900;

		static char photo_name[60];

		if (sec == last_frame_sec) {
			photos_this_sec++;

			sprintf_s(photo_name, 60, "screenshots/Factory Escape on %i-%02i-%02i at %02i.%02i.%02i(%i).png",
				year, month, day, hour, mins, sec, photos_this_sec);
		}
		else {
			photos_this_sec = 0;

			sprintf_s(photo_name, 60, "screenshots/Factory Escape on %i-%02i-%02i at %02i.%02i.%02i.png",
				year, month, day, hour, mins, sec);
		}

		last_frame_sec = sec;

		App->audio->PlayFx(camera_sound_fx);

		LOG("Saving photo %s", photo_name);

		SDL_SaveBMP(App->win->screen_surface, photo_name);
		
	}

	if (return_menu) {

		FreeScene();


		for (p2List_item<UI_Element*>* iterator = App->gui->hud_elements.start; iterator != nullptr; iterator = iterator->next) {

			iterator->data->CleanUp();
		}

		App->gui->hud_elements.clear();

		App->menu->active = true;
		App->menu->need_setup = true;
		App->menu->SetUpMenu();

		Pause = return_menu = App->menu->Started = App->scene->active = App->map->active = false;
		cont_timer_pause = 0u;
		currmap = 1;
	}
	if (need_clean) {
		if (in_game_menu) {
			in_game_menu = need_clean = can_quit = false;
			OpenInGameMenu();
		}

		else if (in_game_settings) {
			need_clean = can_quit = false;
			OpenInGameSettings();
		}

		else if (in_game_save) {
			need_clean = can_quit = false;
			OpenInGameSave();
		}

		else if (in_game_options) {
			need_clean = can_quit = false;
			OpenInGameConfig();
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && !can_quit) {

		if (in_game_menu) {
			can_quit = true;
			in_game_menu = false;
			Pause = false;
			cont_timer_pause += Timer_Pause.ReadSec();
			StartCont_Menu = false;
			DeleteMenu();
		}

		else if (in_game_settings) {
			in_game_settings = false;
			OpenInGameMenu();
		}

		else if (in_game_save) {
			in_game_save = false;
			OpenInGameSettings();
		}

		else if (in_game_options) {
			in_game_options = false;
			OpenInGameSettings();
		}
	}


	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{

	LOG("Freeing scene");
	return true;
}

void j1Scene::FreeScene(bool is_load) {

	DeleteMenu();
	App->map->CleanUp();
	App->audio->FreeMusic();
	App->tex->FreeTextures();
	if(!is_load)
	App->entities->FreeEnemies();
	in_game_menu = false;

}

bool j1Scene::LoadScene(int map, bool is_load) {

	Timer_play.Start();
	cont_timer_pause = 0u;
	bool ret = false;

	FreeScene(is_load);

	if (map == -1) {

		if (CurrentMap->next != nullptr) {
			CurrentMap = CurrentMap->next;
			currmap++;
		}
		else {
			CurrentMap = MapsList_String.start;
			currmap = 1;
		}
	}
	else {
		CurrentMap = MapsList_String.start;
		for (int i = 1; i < map; i++) {
			if (CurrentMap->next != nullptr) {
				CurrentMap = CurrentMap->next;
				currmap = map;
			}
			else 
				break;
			}
	
	}

	ret = App->map->Load(CurrentMap->data.GetString());

	if (ret) {
		static char score_text[3];
		sprintf_s(score_text, 3, "%02i", score_nums);
		score->Define({ 156,178,43,44 }, score_text, { 61,178,44,44 });

		LoadWalkabilityMap();

		if (!is_load) {
			App->scene->FindEntities();

			App->entities->player->FindSpawn();
			App->entities->player->SpawnPlayer();
		}
	}

	return ret;
}


// Load
bool j1Scene::Load(pugi::xml_node&  savegame) {
	currmap = savegame.child("Map").attribute("CurrentMap").as_int();
	score_nums = savegame.child("score").attribute("score").as_uint();
	time_of_start = savegame.child("timer").attribute("time").as_int();
	LOG("%i", time_of_start);
	App->entities->FreeEnemies();

	need_load_scene = true;

	return true;
}

//Save
bool j1Scene::Save(pugi::xml_node& data) const {

	pugi::xml_node map = data.append_child("Map");
	pugi::xml_node score = data.append_child("score");
	pugi::xml_node timer = data.append_child("timer");

	map.append_attribute("CurrentMap") = currmap;
	score.append_attribute("score") = score_nums;
	timer.append_attribute("time") = in_game_time;

	return true;
}

void j1Scene::LoadWalkabilityMap() {
	int w, h;
	uchar* data = NULL;

	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);
}


void j1Scene::FindEntities()
{
	p2List_item<MapLayer*>* layer = App->map->data.layers.end;
	if (layer != nullptr) {
		for (int i = 0; i < layer->data->size_data; i++)
		{
			if (layer->data->data[i] == Tile_Type::BAT_SPAWN)
			{
				iPoint spawn = App->map->TileToWorld(i);
				App->entities->AddEntity(ENTITY_TYPES::BAT, spawn.x, spawn.y);
			}
			if (layer->data->data[i] == Tile_Type::BLOP_SPAWN)
			{
				iPoint spawn = App->map->TileToWorld(i);
				App->entities->AddEntity(ENTITY_TYPES::BLOP, spawn.x, spawn.y);
			}
			if (layer->data->data[i] == Tile_Type::PLAYER_SPAWN)
			{
				iPoint spawn = App->map->TileToWorld(i);
				App->entities->AddEntity(ENTITY_TYPES::PLAYER, spawn.x, spawn.y);
			}
			if (layer->data->data[i] == Tile_Type::MECHANICAL_NUT_SPAWN)
			{
				iPoint spawn = App->map->TileToWorld(i);
				App->entities->AddEntity(ENTITY_TYPES::MECHANICAL_NUT, spawn.x, spawn.y);
			}
		}
	}
}

void j1Scene::CreatePauseMenu() {

	
	window->Define({ 382,124,487,461 }, "");
	//window->image = (Image*)App->gui->AdUIElement(0, 0, IMAGE);
	window->image->LoadUI_Image({ 30,30,275,100 });
	window = (Window*)App->gui->AdUIElement(300, 160, WINDOW);

	Exit = (Button*)App->gui->AdUIElement(420, 260, BUTTON);
	Exit->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "RETURN");
	Exit->TAB = -1;
	Exit->AddListener(this);

	Settings = (Button*)App->gui->AdUIElement(420, 350, BUTTON);
	Settings->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "SETTINGS");
	Settings->TAB = -1;
	Settings->AddListener(this);

	Close = (Button*)App->gui->AdUIElement(420, 440, BUTTON);
	Close->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "CLOSE");
	Close->TAB = -1;
	Close->AddListener(this);

}

void j1Scene::CreateSettingsMenu() {

	window = (Window*)App->gui->AdUIElement(300, 160, WINDOW);
	window->Define({ 382,124,487,461 }, "");

	SaveLoad = (Button*)App->gui->AdUIElement(420, 260, BUTTON);
	SaveLoad->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "SAVE");
	SaveLoad->TAB = -1;
	SaveLoad->AddListener(this);

	Config = (Button*)App->gui->AdUIElement(420, 350, BUTTON);
	Config->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "CONFIG");
	Config->TAB = -1;
	Config->AddListener(this);

	Back = (Button*)App->gui->AdUIElement(420, 440, BUTTON);
	Back->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "BACK");
	Back->TAB = -1;
	Back->AddListener(this);

}

void j1Scene::CreateSaveMenu() {

	window = (Window*)App->gui->AdUIElement(300, 160, WINDOW);
	window->Define({ 382,124,487,461 }, "");

	Save1 = (Button*)App->gui->AdUIElement(420, 260, BUTTON);
	Save1->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "SAVE GAME 1");
	Save1->TAB = -1;
	Save1->AddListener(this);

	Save2 = (Button*)App->gui->AdUIElement(420, 350, BUTTON);
	Save2->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "SAVE GAME 2");
	Save2->TAB = -1;
	Save2->AddListener(this);

	Save3 = (Button*)App->gui->AdUIElement(420, 440, BUTTON);
	Save3->Define(App->gui->button_idle, App->gui->button_hovering, App->gui->button_onclick, "SAVE GAME 3");
	Save3->TAB = -1;
	Save3->AddListener(this);


}

void j1Scene::CreateOptionsMenu() {

	window = (Window*)App->gui->AdUIElement(300, 160, WINDOW);
	window->Define({ 382,124,487,461 }, "");

	text_volum = (Label*)App->gui->AdUIElement(450, 300, LABEL);
	text_volum->SetText("VOLUME");

	min_vol = (Label*)App->gui->AdUIElement(350, 350, LABEL);
	min_vol->SetText("0");

	max_vol = (Label*)App->gui->AdUIElement(700, 350, LABEL);
	max_vol->SetText("100");

	curr_vol = (Label*)App->gui->AdUIElement(575, 300, LABEL);
	static char vol_text[5];
	sprintf_s(vol_text, 5, "%.0f", App->audio->volume * 100);
	curr_vol->SetText(vol_text);


	text_frames = (Label*)App->gui->AdUIElement(450, 450, LABEL);
	text_frames->SetText("FPS");

	min_frames = (Label*)App->gui->AdUIElement(350, 500, LABEL);
	min_frames->SetText("30");

	max_frames = (Label*)App->gui->AdUIElement(700, 500, LABEL);
	max_frames->SetText("240");

	curr_frames = (Label*)App->gui->AdUIElement(575, 450, LABEL);
	static char fps_text[5];
	sprintf_s(fps_text, 5, "%i", App->current_framerate_cap);
	curr_frames->SetText(fps_text);


	volume = (Slider*)App->gui->AdUIElement(375, 350, SLIDER);
	volume->Define({ 31,275,321,20 }, { 260,198,25,25 });
	volume->AddListener(this);
	volume->SetRelativePos(App->audio->volume);

	frames = (Slider*)App->gui->AdUIElement(375, 500, SLIDER);
	frames->Define({ 31,275,321,20 }, { 260,198,25,25 });
	frames->AddListener(this);
	float fps = App->current_framerate_cap - 30;
	frames->SetRelativePos((float)fps / 210);


}

void j1Scene::DeleteMenu() {

	for (p2List_item<UI_Element*>* iterator = App->gui->ui_elements.start; iterator != nullptr; iterator = iterator->next) {
		iterator->data->CleanUp();
	}
	App->gui->ui_elements.clear();

}

void j1Scene::GUICallback(UI_Element* element) {

	if (element == pause_butt)
		click_PauseButt = true;

	else if (in_game_menu) {

		if (Exit == element)
			return_menu = true;

		else if (Settings == element) {
			in_game_settings = need_clean = true;
			in_game_menu = false;
		}

		else if (Close == element) {
			cont_timer_pause += Timer_Pause.ReadSec();
			StartCont_Menu = false;
			Pause = false;
			can_quit = true;
			in_game_menu = false;
			DeleteMenu();
		}
	}

	else if (in_game_settings) {

		if (SaveLoad == element) {
			in_game_save = need_clean = true;
			in_game_settings = false;
		}

		else if (Config == element) {
			in_game_options = need_clean = true;
			in_game_settings = false;
		}
		else if (Back == element) {
			in_game_menu = need_clean = true;
			in_game_settings = false;
		}
	}

	else if (in_game_save) {

		if (Save1 == element) {
			need_save = true;
			save_game = 0;
			App->gui->active = false;
		}

		else if (Save2 == element) {
			need_save = true;
			save_game = 1;
			App->gui->active = false;
		}
		else if (Save3 == element) {
			need_save = true;
			save_game = 2;
			App->gui->active = false;
		}
	}
	else if (in_game_options) {

		if (frames == element) {

			float fps = frames->GetRelativePosition();
			fps = (fps * 210) + 30;
			App->framerate_cap = App->current_framerate_cap = fps;
			static char frames_text[5];

			sprintf_s(frames_text, 5, "%.0f", fps);
			curr_frames->SetText(frames_text);
		}
		else if (volume == element) {

			float vol = volume->GetRelativePosition();
			App->audio->SetVolume(vol);
			static char vol_text[4];
			sprintf_s(vol_text, 4, "%.0f", vol * 100);
			curr_vol->SetText(vol_text);
		}
	}


}

void j1Scene::OpenInGameMenu() {

	if (!in_game_menu) {
		DeleteMenu();

		Pause = true;
		in_game_settings = in_game_save = in_game_options = false;
		in_game_menu = true;

		CreatePauseMenu();
	}
}

void j1Scene::OpenInGameSettings() {

	DeleteMenu();

	in_game_menu = in_game_save = in_game_options = false;
	in_game_settings = true;

	CreateSettingsMenu();

}

void j1Scene::OpenInGameSave() {

	DeleteMenu();

	in_game_menu = in_game_settings = in_game_options = false;
	in_game_save = true;

	CreateSaveMenu();

}

void j1Scene::OpenInGameConfig() {

	DeleteMenu();
	in_game_menu = in_game_save = in_game_settings = false;
	in_game_options == true;

	CreateOptionsMenu();

}
