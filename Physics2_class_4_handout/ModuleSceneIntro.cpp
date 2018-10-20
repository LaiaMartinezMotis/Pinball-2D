#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ChainList.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Initialize textures & audio
	circle = App->textures->Load("pinball/wheel.png"); 
	scenario = App->textures->Load("pinball/scenario.png");
	ball = App->textures->Load("pinball/ball.png");
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
	App->textures->Unload(scenario);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//Print background layer
	App->renderer->Blit(scenario, 0, 0, NULL, 1.0f);

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

	if (pb_ball != NULL)
	{
		int x, y;
		pb_ball->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, pb_ball->GetRotation());
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(bonus_fx);
}

//Load Map
bool ModuleSceneIntro::LoadMap()
{

	pb_background = App->physics->CreateChain(0, 0, scenario_points, 320);
	pb_background->body->SetType(b2_staticBody);

	pb_background_elements.add(App->physics->CreateChain(0, 0, wall_10000, 14));
	pb_background_elements.add(App->physics->CreateChain(0, 0, wall_launch_ramp, 58));
	pb_background_elements.add(App->physics->CreateChain(0, 0, left_bumper, 22));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_bumper, 20));
	pb_background_elements.add(App->physics->CreateChain(0, 0, left_slingshot, 16));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_slingshot, 16));
	pb_background_elements.add(App->physics->CreateChain(0, 0, right_blocker, 10));
	pb_background_elements.add(App->physics->CreateChain(0, 0, up_bumper, 42));
	pb_background_elements.add(App->physics->CreateChain(0, 0, middle_bumper_1, 42));
	pb_background_elements.add(App->physics->CreateChain(0, 0, middle_bumper_2, 44));
	pb_background_elements.add(App->physics->CreateChain(0, 0, middle_bumper_3, 44));
	pb_background_elements.add(App->physics->CreateChain(0, 0, up_blocker_1, 18));
	pb_background_elements.add(App->physics->CreateChain(0, 0, up_blocker_2, 18));
	p2List_item<PhysBody*>* back_elem = pb_background_elements.getFirst();
	while (back_elem != NULL)
	{
		back_elem->data->body->SetType(b2_staticBody);
		back_elem = back_elem->next;
	}

	return true;
}