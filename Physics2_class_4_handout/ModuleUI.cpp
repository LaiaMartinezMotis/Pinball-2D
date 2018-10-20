#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading UI");
	score_font = App->fonts->Load("pinball/score_font.png" ,"0123456789", 1);
	return true;
}

// Unload assets
bool ModuleUI::CleanUp()
{
	LOG("Unloading UI");

	return true;
}

// Update: draw background
update_status ModuleUI::Update()
{
	App->fonts->BlitText(0, 0, score_font, "0123459876");
	return UPDATE_CONTINUE;
}



