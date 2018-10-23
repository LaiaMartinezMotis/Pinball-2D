#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleUI.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ChainList.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
	circle = NULL;
	arrows_left.PushBack({ 58,83,29,34 });
	arrows_left.PushBack({ 29,83,29,34 });
	arrows_left.PushBack({ 0,83,29,34 });
	arrows_left.speed = 0.08F;

	arrows_right.PushBack({ 62,117,31,36 });
	arrows_right.PushBack({ 31,117,31,36 });
	arrows_right.PushBack({ 0,117,31,36 });
	arrows_right.speed = 0.08F;

	arrows_down.PushBack({ 0, 153, 22, 39 });
	arrows_down.PushBack({ 23, 153, 22, 39 });
	arrows_down.PushBack({ 46, 153, 22, 39 });
	arrows_down.speed = 0.08F;
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Initialize textures & audio
	circle = App->textures->Load("pinball/wheel.png"); 
	scenario = App->textures->Load("pinball/scenario.png");
	upper_scenario = App->textures->Load("pinball/upper_scenario.png");
	ball = App->textures->Load("pinball/ball.png");
	background_elements = App->textures->Load("pinball/background_elements.png");


	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	lv2_fx = App->audio->LoadFx("pinball/lv2.wav");
	bumper_fx = App->audio->LoadFx("pinball/bumper_fx.wav");
	littlebumper_fx=App->audio->LoadFx("pinball/littlebump_fx.wav");
	lights_fx = App->audio->LoadFx("pinball/lights_fx.wav");
	death_fx = App->audio->LoadFx("pinball/death_fx.wav");
	arrows_fx = App->audio->LoadFx("pinball/redarrows_fx.wav");

	//Create Physbodys
	ret = LoadMap();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(circle);
	App->textures->Unload(ball);
	App->textures->Unload(background_elements);
	App->textures->Unload(scenario);
	return true;
}
update_status ModuleSceneIntro::PreUpdate()
{
	//Flippers Movement
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		
		pb_left_flipper->body->ApplyAngularImpulse(-2.0F, true);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		
		pb_right_flipper->body->ApplyAngularImpulse(2.0F, true);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) 
	{
		plunger_joint->SetMotorSpeed((0.0F, 1.0F));
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		plunger_joint->SetMotorSpeed((0.0F, -40.0F));
	}
	

	return UPDATE_CONTINUE;
}
// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//Print background layer
	App->renderer->Blit(scenario, 0, 0, NULL, 1.0f);
	App->renderer->Blit(background_elements, 174, 435, &arrows_left.GetCurrentFrame(), 1.0F);
	App->renderer->Blit(background_elements, 309, 366, &arrows_right.GetCurrentFrame(), 1.0F);
	App->renderer->Blit(background_elements, 370, 546, &arrows_down.GetCurrentFrame(), 1.0F);


	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		pb_ball = (App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10));
		pb_ball->listener = this;
	}

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	if (pb_right_flipper != NULL)
	{
		int x, y;
		pb_right_flipper->GetPosition(x, y);
		SDL_Rect rect = { 0,45,69,36 };
		App->renderer->Blit(background_elements, 235, 740, &rect, 1.0f, pb_right_flipper->GetRotation() + 20, 65, 10);
	}

	if (pb_left_flipper != NULL)
	{
		int x, y;
		pb_left_flipper->GetPosition(x, y);
		SDL_Rect rect = { 0,0,68,36 };
		App->renderer->Blit(background_elements, 150, 740, &rect, 1.0f, pb_left_flipper->GetRotation() - 20, 10, 10);
	}

	if (pb_plunger != NULL)
	{
		int x, y;
		pb_plunger->GetPosition(x, y);
		SDL_Rect rect = { 96,83,19,80 };
		App->renderer->Blit(background_elements, x + 412, y + 651, &rect, 1.0f);
	}

	if (pb_right_bumper != NULL && pb_right_bumper->light == true)
	{
		int x, y;
		pb_right_bumper->GetRealPosition(x, y);
		SDL_Rect rect = {135,192,65,125};
		App->renderer->Blit(background_elements, x, y, &rect);

		Timer(pb_right_bumper, 400);
	}

	if (pb_left_bumper != NULL && pb_left_bumper->light == true)
	{
		int x, y;
		pb_left_bumper->GetRealPosition(x, y);
		SDL_Rect rect = { 135,78,66,112 };
		App->renderer->Blit(background_elements, x, y, &rect);

		Timer(pb_left_bumper, 400);
	}

	if (pb_launch_ramp != NULL && pb_launch_ramp->light == true)
	{
		int x, y;
		pb_launch_ramp->GetRealPosition(x, y);
		SDL_Rect rect = { 72,2,92,69 };
		App->renderer->Blit(background_elements, x, y, &rect);

		Timer(pb_launch_ramp, 200);
	}

	if (pb_ramp != NULL && pb_ramp->light == true)
	{
		int x, y;
		pb_ramp->GetRealPosition(x, y);
		SDL_Rect rect = { 69, 266, 66, 100 };
		App->renderer->Blit(background_elements, x, y, &rect);

		Timer(pb_ramp, 200);
	}

	if (pb_ball != NULL)
	{
		int x, y;
		pb_ball->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, pb_ball->GetRotation());
	}

	//Check if light = true, draw collision sprite
	p2List_item<PhysBody*>* bumper = pb_bumpers.getFirst();
	while (bumper != NULL)
	{
		if (bumper->data->light == true)
		{
			int x, y;
			bumper->data->GetRealPosition(x, y);
			SDL_Rect rect = { 8,233,61,61 };
			App->renderer->Blit(background_elements, x, y, &rect, 1.0f, bumper->data->GetRotation());
			Timer(bumper->data, 200);
		}
		bumper = bumper->next;
	}

	p2List_item<PhysBody*>* little_bumper = pb_little_bumpers.getFirst();
	while (little_bumper != NULL)
	{
		if (little_bumper->data->light == true)
		{
			int x, y;
			little_bumper->data->GetPosition(x, y);
			SDL_Rect rect = { 167,2,34,34 };
			App->renderer->Blit(background_elements, x, y - 2, &rect, 1.0f, little_bumper->data->GetRotation());
			Timer(little_bumper->data, 200);
		}
		little_bumper = little_bumper->next;
	}

	if (destroy)
	{
		pb_ball->body->GetWorld()->DestroyBody(pb_ball->body);
		pb_ball = (App->physics->CreateCircle(414, 626, 10));
		pb_ball->listener = this;
		life--;
		destroy = false;
	}

	App->renderer->Blit(upper_scenario, 0, 0, NULL, 1.0f);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//App->audio->PlayFx(bonus_fx);
	if (bodyA == pb_ball)
	{
		//Lights
		p2List_item<PhysBody*>* red_item = pb_red_lights.getFirst();
		while (red_item != NULL) {
			if (bodyB == red_item->data)
			{
				/*App->audio->PlayFx(lights_fx);*/
				App->ui->score_player += 200;
			}
			red_item = red_item->next;
		}

		p2List_item<PhysBody*>* bumper_item = pb_bumpers.getFirst();
		while (bumper_item != NULL) {
			if (bodyB == bumper_item->data)
			{
				/*App->audio->PlayFx(bumper_fx);*/
				App->ui->score_player += 500;
				bumper_item->data->light = true;
			}
			bumper_item = bumper_item->next;
		}
		p2List_item<PhysBody*>* arrow_item = pb_arrow_lights.getFirst();
		while (arrow_item != NULL) {
			if (bodyB == arrow_item->data)
			{
				/*App->audio->PlayFx(arrows_fx);*/
				
				arrow_item->data->light = true;
			}
			arrow_item = arrow_item->next;
		}
		
		p2List_item<PhysBody*>* little_bumper_item = pb_little_bumpers.getFirst();
		while (little_bumper_item != NULL) {
			if (bodyB == little_bumper_item->data)
			{
				/*App->audio->PlayFx(littlebumper_fx);*/
				App->ui->score_player += 500;
				little_bumper_item->data->light = true;
			}
			little_bumper_item = little_bumper_item->next;
		}

		if (bodyB == pb_left_bumper)
		{
			/*App->audio->PlayFx(bonus_fx);*/
			pb_left_bumper->light = true;
		}
		if (bodyB == pb_right_bumper)
		{
			/*App->audio->PlayFx(bonus_fx);*/
			pb_right_bumper->light = true;
		}
		if (bodyB == pb_launch_ramp)
		{
			pb_launch_ramp->light = true;
		}
		if (bodyB == pb_ramp)
		{
			pb_ramp->light = true;
		}
		if (bodyB == pb_right_push || bodyB == pb_left_push)
		{
			bodyA->body->ApplyLinearImpulse({0.0f,-2.0F}, bodyA->body->GetLocalCenter(), true);
		}
		
		//Death
		if (bodyB == pb_death_sensor)
		{
			/*App->audio->PlayFx(death_fx);*/
			destroy = true;
		}
	}
}

//Load Map
bool ModuleSceneIntro::LoadMap()
{
	//Create Ball
	pb_ball = (App->physics->CreateCircle(414, 626, 10));
	pb_ball->body->SetBullet(true);
	pb_ball->listener = this;
	life = 4;

	//Sensor light

	//Define Physbodys
	pb_background = App->physics->CreateChain(0, 0, scenario_points, 318);
	pb_background->body->SetType(b2_staticBody);

	pb_purple_ramp = App->physics->CreateChain(0, 0, purple_ramp, 62);
	pb_purple_ramp->body->SetType(b2_staticBody);

	pb_right_flipper = App->physics->CreateRectangle(270, 746, 62, 13);
	pb_left_flipper = App->physics->CreateRectangle(180,750,62,13);

	pb_right_bumper = App->physics->CreateChain(0, 0, right_bumper_coll, 6, 277, 606);
	pb_right_bumper->body->SetType(b2_staticBody);
	pb_right_bumper->body->GetFixtureList()->SetRestitution(1.0F);
	pb_right_bumper->body->GetFixtureList()->SetFriction(0.4F);

	pb_left_bumper = App->physics->CreateChain(0, 0, left_bumper_coll, 6, 105, 605);
	pb_left_bumper->body->SetType(b2_staticBody);
	pb_left_bumper->body->GetFixtureList()->SetRestitution(1.0F);
	pb_left_bumper->body->GetFixtureList()->SetFriction(0.4F);

	pb_plunger = App->physics->CreateChain(0, 0, plunger, 10);

	pb_background_elements.add(App->physics->CreateChain(0, 0, wall_10000, 14));
	pb_background_elements.add(App->physics->CreateChain(0, 0, wall_launch_ramp, 56));
	pb_background_elements.add(App->physics->CreateChain(0, 0, left_bumper, 22));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_bumper, 20));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_blocker, 10));
	pb_background_elements.add(App->physics->CreateChain(0, 0, up_blocker_1, 18));
	pb_background_elements.add(App->physics->CreateChain(0, 0, up_blocker_2, 18));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_slingshot, 16));
	pb_background_elements.add(App->physics->CreateChain(0, 0, left_slingshot, 16));
	pb_background_elements.add(App->physics->CreateChain(0, 0, left_slingshot, 16));
	pb_background_elements.add(App->physics->CreateRectangle(33, 423, 6, 18));
	pb_background_elements.add(App->physics->CreateRectangle(64, 436, 6, 18));

	p2List_item<PhysBody*>* back_elem = pb_background_elements.getFirst();
	while (back_elem != NULL)
	{
		back_elem->data->body->SetType(b2_staticBody);
		back_elem = back_elem->next;
	}

	//Define Bumper Physbodys

	pb_launch_ramp = App->physics->CreateChain(0, 0, launch_ramp_sensor, 8, 77, 277);
	pb_launch_ramp->body->SetType(b2_staticBody);
	pb_launch_ramp->body->GetFixtureList()->SetRestitution(1.0F);
	pb_launch_ramp->body->GetFixtureList()->SetFriction(0.2F);

	pb_ramp = App->physics->CreateChain(0, 0, ramp_sensor, 6, 275, 239);
	pb_ramp->body->SetType(b2_staticBody);
	pb_ramp->body->GetFixtureList()->SetRestitution(1.0F);
	pb_ramp->body->GetFixtureList()->SetFriction(0.2F);

	pb_bumpers.add(App->physics->CreateChain(0, 0, up_bumper, 42, 40, 40));
	pb_bumpers.add(App->physics->CreateChain(0, 0, middle_bumper_1, 42, 147, 195));
	pb_bumpers.add(App->physics->CreateChain(0, 0, middle_bumper_2, 44, 225, 165));
	pb_bumpers.add(App->physics->CreateChain(0, 0, middle_bumper_3, 44, 181, 267));

	pb_little_bumpers.add(App->physics->CreateCircle(23, 465, 15));
	pb_little_bumpers.add(App->physics->CreateCircle(80, 479, 15));
	pb_little_bumpers.add(App->physics->CreateCircle(42, 514, 15));

	p2List_item<PhysBody*>* little_bump_elem = pb_little_bumpers.getFirst();
	while (little_bump_elem != NULL)
	{
		little_bump_elem->data->body->SetType(b2_staticBody);
		little_bump_elem->data->body->GetFixtureList()->SetRestitution(1.0F);
		little_bump_elem->data->body->GetFixtureList()->SetFriction(0.4F);
		little_bump_elem = little_bump_elem->next;
	}

	p2List_item<PhysBody*>* bump_elem = pb_bumpers.getFirst();
	while (bump_elem != NULL)
	{
		bump_elem->data->body->SetType(b2_staticBody);
		bump_elem->data->body->GetFixtureList()->SetRestitution(1.0F);
		bump_elem->data->body->GetFixtureList()->SetFriction(0.2F);
		bump_elem = bump_elem->next;
	}
	
	//Define Sensors
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 570, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 455, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 340, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 230, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(405, 150, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(350, 95, 15, 40));

	pb_arrow_lights.add(App->physics->CreateRectangleSensor(186, 448, 15, 34));
	

	pb_arrow_lights.add(App->physics->CreateRectangleSensor(309, 366, 15, 36));
	

	pb_arrow_lights.add(App->physics->CreateRectangleSensor(370, 546, 15, 39));
	

	pb_death_sensor = App->physics->CreateRectangleSensor(220, 805, 80, 20);
	pb_left_push = App->physics->CreateRectangleSensor(25,772,15,16);
	pb_right_push = App->physics->CreateRectangleSensor(384,772,16,16);

	//Define Joints
	//Revolute Joint
	b2RevoluteJointDef left_flipper;
	b2RevoluteJointDef right_flipper;
	PhysBody* circle_left;
	circle_left = App->physics->CreateCircle(155, 745, 5);
	circle_left->body->SetType(b2_staticBody);

	PhysBody* circle_right;
	circle_right = App->physics->CreateCircle(300, 745, 5);
	circle_right->body->SetType(b2_staticBody);

	left_flipper.Initialize(pb_left_flipper->body, circle_left->body, circle_left->body->GetWorldCenter());
	left_flipper.collideConnected = false;

	right_flipper.Initialize(pb_right_flipper->body, circle_right->body, circle_right->body->GetWorldCenter());
	right_flipper.collideConnected = false;

	left_flipper.enableLimit = true;
	left_flipper.lowerAngle = -20 * DEGTORAD;
	left_flipper.upperAngle = 30 * DEGTORAD;

	right_flipper.enableLimit = true;
	right_flipper.lowerAngle = -20 * DEGTORAD;
	right_flipper.upperAngle = 20 * DEGTORAD;


	left_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&left_flipper);
	right_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&right_flipper);

	//Prismatic Joint
	b2PrismaticJointDef jointDef; 
	b2Vec2 worldAxis(0.0f, 1.0f); 
	jointDef.Initialize(App->physics->ground, pb_plunger->body, { 422, 742 }, worldAxis);

	jointDef.enableLimit = true;
	jointDef.upperTranslation = 1.0F;
	jointDef.lowerTranslation = 0.0F;

	jointDef.enableMotor = true;
	jointDef.maxMotorForce = 500.0f;
	jointDef.motorSpeed = 0.0f;

	plunger_joint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&jointDef);
	return true;
}

void ModuleSceneIntro::Timer(PhysBody* pb, int time)
{
	if (timer)
	{
		time_on_entry = SDL_GetTicks();
		timer = false;
	}
	current_time = SDL_GetTicks() - time_on_entry;
	if (current_time > time)
	{
		pb->light = false;
		timer = true;
	}
}