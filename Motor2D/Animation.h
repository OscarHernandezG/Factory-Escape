#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 10

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float current_frame;
	int last_frame = 0;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		
		current_frame += speed * 5 * App->dt;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool isFinished() const
	{
		bool ret = false;
		if (current_frame >= last_frame-1) {
			ret = true;
		}
		return ret;
	}

	bool Finished() const
	{
		return loops > 0;
	}
	bool ResetLoops() {
		loops = 0;
		return true;
	}

	void Reset()
	{
		current_frame = 0;
	}
	void Stop()
	{
		speed = 0;
	}
};

#endif