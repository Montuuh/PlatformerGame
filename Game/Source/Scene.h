#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;
class GuiControl;

class Scene : public Module
{
public:

	Scene();

	Scene(bool b);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

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

	virtual bool OnGuiMouseClickEvent(GuiControl* control);

	int counterSeconds = 0;

private:
	SDL_Texture* img;
	SDL_Texture* bg;

	int storeFrameRateCap;
	bool capFrameRate = true;
	

public:
	
	bool loaded = false;

};

#endif // __SCENE_H__