#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

private:
	bool LoadMap();
	void Timer(PhysBody* pb, int time);

public:
	p2List<PhysBody*>	circles;
	p2List<PhysBody*>	pb_background_elements;
	p2List<PhysBody*>	pb_red_lights;
	p2List<PhysBody*>	pb_bumpers;

	PhysBody*			pb_ball;
	PhysBody*			pb_background;
	PhysBody*			pb_right_flipper;
	PhysBody*			pb_left_flipper;
	PhysBody*           pb_right_bumper;
	PhysBody*           pb_left_bumper;
	PhysBody*			pb_plunger;
	PhysBody*			pb_launch_ramp;
	PhysBody*			pb_ramp;

	PhysBody*			pb_death_sensor;
	
	b2RevoluteJoint*    right_joint;
	b2RevoluteJoint*    left_joint;
	b2PrismaticJoint*   plunger_joint;
	
	SDL_Texture* circle = nullptr;
	SDL_Texture* ball = nullptr;
	SDL_Texture* scenario = nullptr;
	SDL_Texture* upper_scenario = nullptr;
	SDL_Texture* background_elements = nullptr;
	uint bonus_fx;

	p2Animation arrows_left;
	p2Animation arrows_right;
	p2Animation arrows_down;

	bool timer = true;
	int time_on_entry = 0;
	int current_time = 0;

	bool destroy = false;
	int life = 0;

};
