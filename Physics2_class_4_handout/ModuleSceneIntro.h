#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

private:
	bool LoadMap();

public:
	p2List<PhysBody*>	circles;
	p2List<PhysBody*>	pb_background_elements;
	PhysBody*			pb_ball;
	PhysBody*			pb_background;
	PhysBody*			pb_right_flipper;
	PhysBody*			pb_left_flipper;

	SDL_Texture* circle;
	SDL_Texture* ball;
	SDL_Texture* scenario;
	SDL_Texture* background_elements;
	uint bonus_fx;
};
