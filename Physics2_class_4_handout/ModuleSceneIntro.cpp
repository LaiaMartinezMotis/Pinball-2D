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
		
		pb_left_flipper->body->ApplyAngularImpulse(-3.0F, true);
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
	

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
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

	while(c != NULL)
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
		App->renderer->Blit(background_elements, x + 235, y + 741, &rect, 1.0f, pb_right_flipper->GetRotation());
	}

	if (pb_left_flipper != NULL)
	{
		int x, y;
		pb_left_flipper->GetPosition(x, y);
		SDL_Rect rect = { 0,0,68,36 };
		App->renderer->Blit(background_elements, x + 146, y + 741, &rect, 1.0f, pb_left_flipper->GetRotation());
	}

	if (pb_plunger != NULL)
	{
		int x, y;
		pb_plunger->GetPosition(x, y);
		SDL_Rect rect = {96,83,19,80};
		App->renderer->Blit(background_elements, x + 412, y + 650, &rect, 1.0f);
	}

	if (pb_ball != NULL)
	{
		int x, y;
		pb_ball->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, pb_ball->GetRotation());
	}

	App->renderer->Blit(upper_scenario, 0, 0, NULL, 1.0f);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//App->audio->PlayFx(bonus_fx);
	if (bodyA == pb_ball)
	{
		p2List_item<PhysBody*>* red_item = pb_red_lights.getFirst();
		while (red_item != NULL) {
			if (bodyB == red_item->data)
			{
				App->ui->score_player += 200;
			}
			red_item = red_item->next;
		}

		p2List_item<PhysBody*>* bump_item = pb_bumpers.getFirst();
		while (bump_item != NULL) {
			if (bodyB == bump_item->data)
			{
				App->ui->score_player += 500;
			}
			bump_item = bump_item->next;
		}
	}
}

//Load Map
bool ModuleSceneIntro::LoadMap()
{
	//Define Physbodys
	pb_background = App->physics->CreateChain(0, 0, scenario_points, 320);
	pb_background->body->SetType(b2_staticBody);

	pb_right_flipper = App->physics->CreateRectangle(230, 740, 20, 15);
	pb_right_flipper->body->SetType(b2_dynamicBody);

	pb_left_flipper = App->physics->CreateRectangle(180,750,60,15);
	pb_left_flipper->body->SetType(b2_dynamicBody);

	pb_right_slingshot = App->physics->CreateChain(0, 0, right_slingshot, 16);
	pb_right_slingshot->body->SetType(b2_staticBody);
	pb_left_slingshot = App->physics->CreateChain(0, 0, left_slingshot, 16);
	pb_left_slingshot->body->SetType(b2_staticBody);

	pb_plunger = App->physics->CreateChain(0, 0, plunger, 10);

	pb_background_elements.add(App->physics->CreateChain(0, 0, wall_10000, 14));
	pb_background_elements.add(App->physics->CreateChain(0, 0, wall_launch_ramp, 58));
	pb_background_elements.add(App->physics->CreateChain(0, 0, left_bumper, 22));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_bumper, 20));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_blocker, 10));
	pb_background_elements.add(App->physics->CreateChain(0, 0, up_blocker_1, 18));
	pb_background_elements.add(App->physics->CreateChain(0, 0, up_blocker_2, 18));

	p2List_item<PhysBody*>* back_elem = pb_background_elements.getFirst();
	while (back_elem != NULL)
	{
		back_elem->data->body->SetType(b2_staticBody);
		back_elem = back_elem->next;
	}

	//Define Bumper Physbodys

	pb_ramp_sensor = App->physics->CreateChain(0, 0, ramp_sensor, 8);
	pb_ramp_sensor->body->SetType(b2_staticBody);
	pb_ramp_sensor->body->GetFixtureList()->SetRestitution(1.0F);

	pb_bumpers.add(App->physics->CreateChain(0, 0, up_bumper, 42));
	pb_bumpers.add(App->physics->CreateChain(0, 0, middle_bumper_1, 42));
	pb_bumpers.add(App->physics->CreateChain(0, 0, middle_bumper_2, 44));
	pb_bumpers.add(App->physics->CreateChain(0, 0, middle_bumper_3, 44));

	p2List_item<PhysBody*>* bump_elem = pb_bumpers.getFirst();
	while (bump_elem != NULL)
	{
		bump_elem->data->body->SetType(b2_staticBody);
		bump_elem->data->body->GetFixtureList()->SetRestitution(3.5F);
		bump_elem->data->body->GetFixtureList()->SetFriction(1.0F);
		bump_elem = bump_elem->next;
	}

	//Define Sensors
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 570, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 455, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 340, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(420, 230, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(405, 150, 30, 15));
	pb_red_lights.add(App->physics->CreateRectangleSensor(350, 95, 15, 40));

	//Define Joints
	//Revolute Joint
	b2RevoluteJointDef left_flipper;
	b2RevoluteJointDef right_flipper;
	PhysBody* circle_left;
	circle_left = App->physics->CreateCircle(155, 750, 10);
	circle_left->body->SetType(b2_staticBody);

	PhysBody* circle_right;
	circle_right = App->physics->CreateCircle(300, 750, 10);
	circle_right->body->SetType(b2_staticBody);

	left_flipper.Initialize(pb_left_flipper->body, circle_left->body, circle_left->body->GetWorldCenter());
	left_flipper.collideConnected = false;

	right_flipper.Initialize(pb_right_flipper->body, pb_right_slingshot->body, {290, 750});
	right_flipper.collideConnected = false;

	left_flipper.enableLimit = true;
	left_flipper.lowerAngle = -0.15 * b2_pi;
	left_flipper.upperAngle = 0.25 * b2_pi;

	right_flipper.enableLimit = true;
	right_flipper.lowerAngle = -0.5 * b2_pi;
	right_flipper.upperAngle = -0.25 * b2_pi;


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